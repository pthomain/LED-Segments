#include "canvas.h"
#include "utils/utils.h"
#include <utility>
#include <vector>
#include "effects/effect.h"
#include "config/effectconfig.h"

void Canvas::applyConfig(
        EffectConfig *effectConfig
) {
    delete currentEffectConfig;
    currentEffectConfig = effectConfig;

    effectIteration++;
    effectFrameIndex = 0;

    clearEffectOrModifier(effectPerSectionPixels);
    clearEffectOrModifier(modifierPerSectionPixels);

    applyEffectOrModifier(effectPerSectionPixels, effectConfig->effectFactory);
    if (effectConfig->modifierFactory != nullptr) {
        applyEffectOrModifier(modifierPerSectionPixels, *effectConfig->modifierFactory);
    }
}

void Canvas::applyEffectOrModifier(
        std::vector<std::pair<Effect *, std::vector<Section>>> &effectMap,
        const std::function<Effect *(const EffectContext &effectContext)> &effectFactory
) const {
    if (currentEffectConfig == nullptr) return;

    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If pixelUnits is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in pixelUnits and the current section

    for (uint16_t scopeIndex = 0; scopeIndex < currentEffectConfig->scopeCluster.scopeSections.size(); scopeIndex++) {
        auto &scopeSection = currentEffectConfig->scopeCluster.scopeSections.at(scopeIndex);
        if (currentEffectConfig->pixelUnits == nullptr) {
            //If pixelUnits is null, we don't need to apply a mirror or look for intersected sections
            Effect *effect = effectFactory(
                    EffectContext(
                            scopeSection,
                            currentEffectConfig->scopeCluster.scope,
                            SCOPE_WHOLE,
                            MIRROR_NONE,
                            currentEffectConfig->scopeCluster.scopeSections.size() - 1,
                            0,
                            scopeIndex,
                            effectIteration,
                            effectFrameIndex
                    )
            );

            effectMap.emplace_back(effect, emptySections);
        } else {
            auto intersectedSections = intersectAllPixelsWithClusterScope(
                    scopeSection,
                    currentEffectConfig->pixelUnits->scopeSections
            );

            int intersectedSize = intersectedSections.size();
            int sectionSize = currentEffectConfig->mirror == MIRROR_NONE
                              ? intersectedSize
                              : intersectedSize % 2 == 0
                                ? intersectedSize / 2
                                : (intersectedSize + 1) / 2;

            Section pixelSection = Section(0, sectionSize - 1);

            Effect *effect = effectFactory(
                    EffectContext(
                            pixelSection,
                            currentEffectConfig->scopeCluster.scope,
                            currentEffectConfig->pixelUnits->scope,
                            currentEffectConfig->mirror,
                            currentEffectConfig->scopeCluster.scopeSections.size() - 1,
                            intersectedSize - 1,
                            scopeIndex,
                            effectIteration,
                            effectFrameIndex
                    )
            );

            effectMap.emplace_back(effect, intersectedSections);
        }
    }
}

std::vector<Section> Canvas::intersectAllPixelsWithClusterScope(
        const Section &clusterSection,
        const std::vector<Section> &pixelSections
) {
    std::vector<Section> intersectedSections = std::vector<Section>();

    for (Section pixelSection: pixelSections) {
        if (pixelSection.start >= clusterSection.start && pixelSection.end <= clusterSection.end) {
            intersectedSections.push_back(pixelSection);
        }
    }

    return intersectedSections;
}

void Canvas::render(CRGB *outputArray) {
    if (currentEffectConfig == nullptr) return;

    for (int x = 0; x < effectPerSectionPixels.size(); x++) {
        auto effectPair = effectPerSectionPixels.at(x);
        auto &effect = effectPair.first;
        auto &effectPixelSections = effectPair.second;

        auto modifierPair = modifierPerSectionPixels.empty() ? nullptr : &modifierPerSectionPixels.at(x);
        auto modifier = modifierPair == nullptr ? nullptr : modifierPair->first;
        auto modifierPixelSections = modifierPair == nullptr ? emptySections : modifierPair->second;

        if (effectPixelSections.empty()) {
            if (currentEffectConfig->scopeCluster.scope == SCOPE_WHOLE) {
                effect->fillArray(outputArray);
                if (modifier != nullptr) modifier->fillArray(outputArray);
            } else {
                effect->fillArray(effectBufferArray);
                if (modifier != nullptr) {
                    for (int i = 0; i < effect->effectContext.section.sectionSize; i++) {
                        modifierBufferArray[i] = effectBufferArray[i];
                    }
                    modifier->fillArray(modifierBufferArray);
                }
                for (int i = 0; i < effect->effectContext.section.sectionSize; i++) {
                    outputArray[effect->effectContext.section.start + i] = (modifier == nullptr)
                                                             ? effectBufferArray[i]
                                                             : modifierBufferArray[i];
                }
            }
        } else {
            effect->fillArray(effectBufferArray);
            if (modifier != nullptr) {
                for (int i = 0; i < effect->effectContext.section.sectionSize; i++) {
                    modifierBufferArray[i] = effectBufferArray[i];
                }
                modifier->fillArray(modifierBufferArray);
            }

            if (effect->effectContext.mirror == MIRROR_EDGE) {
                int centre = effect->effectContext.section.sectionSize % 2 == 0
                             ? effect->effectContext.section.sectionSize / 2
                             : (effect->effectContext.section.sectionSize + 1) / 2;

                for (int i = 0; i < centre; i++) {
                    int right = centre + i;
                    int left = centre - 1 - i;

                    if (modifier != nullptr) {
                        CRGB temp = modifierBufferArray[right];
                        modifierBufferArray[right] = modifierBufferArray[left];
                        modifierBufferArray[left] = temp;
                    } else {
                        CRGB temp = effectBufferArray[right];
                        effectBufferArray[right] = effectBufferArray[left];
                        effectBufferArray[left] = temp;
                    }
                }
            }

            if (effect->effectContext.mirror != MIRROR_NONE) {
                for (int i = 0; i < effect->effectContext.section.sectionSize; i++) {
                    effectBufferArray[effect->effectContext.section.sectionSize + i] = effectBufferArray[effect->effectContext.section.end - i];
                }
                if (modifier != nullptr) {
                    for (int i = 0; i < effect->effectContext.section.sectionSize; i++) {
                        modifierBufferArray[effect->effectContext.section.sectionSize + i] = modifierBufferArray[
                                effect->effectContext.section.end - i];
                    }
                }
            }

            for (int i = 0; i < effectPixelSections.size(); i++) {
                auto pixelSection = effectPixelSections.at(i);
                for (int j = pixelSection.start; j <= pixelSection.end; j++) {
                    outputArray[j] = (modifier == nullptr) ? effectBufferArray[i] : modifierBufferArray[i];
                }
            }
        }
    }

    effectFrameIndex++;
}

void Canvas::clearEffectOrModifier(std::vector<std::pair<Effect *, std::vector<Section> > > &effectMap) {
    for (const auto &item: effectMap) {
        delete item.first;
    }
    effectMap.clear();
}
