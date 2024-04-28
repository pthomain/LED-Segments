#include "cluster.h"
#include "utils.h"
#include <utility>
#include <vector>
#include <FastLED.h>
#include "effects/effect.h"
#include "config/effectconfig.h"

Cluster::Cluster(std::vector<Section> sections, Scope scope)
        : scope(scope),
          scopeSections(std::move(sections)),
          clusterSection(
                  Section(
                          scopeSections.at(0).start,
                          scopeSections.at(scopeSections.size() - 1).end
                  )
          ) {}

void Cluster::applyConfig(
        const EffectConfig *effectConfig
) {
    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If pixelUnits is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in pixelUnits and the current section

    auto &effectMap = effectConfig->isModifier ? modifierPerSectionPixels : effectPerSectionPixels;
    clearEffectOrModifier(effectMap);

    for (const auto &scopeSection: scopeSections) {
        if (effectConfig->pixelUnits == nullptr) {
            //If pixelUnits is null, we don't need to apply a mirror
            Effect *effect = effectConfig->effectFactory(scopeSection, MIRROR_NONE);
            effectMap.emplace_back(effect, emptySections);
        } else {
            auto intersectedSections = intersectAllPixelsWithClusterScope(
                    scopeSection,
                    effectConfig->pixelUnits->scopeSections
            );

            int intersectedSize = intersectedSections.size();
            int size = effectConfig->mirror == MIRROR_NONE
                       ? intersectedSize
                       : intersectedSize % 2 == 0 ? intersectedSize / 2 : (intersectedSize + 1) / 2;

            Section pixelSection = Section(0, size - 1);

            Effect *effect = effectConfig->effectFactory(pixelSection, effectConfig->mirror);
            effectMap.emplace_back(effect, intersectedSections);
        }
    }
}

std::vector<Section> Cluster::intersectAllPixelsWithClusterScope(
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

void Cluster::render(
        CRGB *targetArray,
        CRGB *effectBufferArray,
        CRGB *modifierBufferArray
) {
    for (int x = 0; x < effectPerSectionPixels.size(); x++) {
        auto effectPair = effectPerSectionPixels.at(x);
        auto &effect = effectPair.first;
        auto &effectPixelSections = effectPair.second;

        auto modifierPair = modifierPerSectionPixels.empty() ? nullptr : &modifierPerSectionPixels.at(x);
        auto modifier = modifierPair == nullptr ? nullptr : modifierPair->first;
        auto modifierPixelSections = modifierPair == nullptr ? emptySections : modifierPair->second;

        if (effectPixelSections.empty()) {
            if (scope == SCOPE_WHOLE) {
                effect->fillArray(targetArray);
                if (modifier != nullptr) modifier->fillArray(targetArray);
            } else {
                effect->fillArray(effectBufferArray);
                if (modifier != nullptr) {
                    for (int i = 0; i < effect->section.sectionSize; i++) {
                        modifierBufferArray[i] = effectBufferArray[i];
                    }
                    modifier->fillArray(modifierBufferArray);
                }
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    targetArray[effect->section.start + i] = (modifier == nullptr) ? effectBufferArray[i]
                                                                                   : modifierBufferArray[i];
                }
            }
        } else {
            effect->fillArray(effectBufferArray);
            if (modifier != nullptr) {
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    modifierBufferArray[i] = effectBufferArray[i];
                }
                modifier->fillArray(modifierBufferArray);
            }

            if (effect->mirror == MIRROR_EDGE) {
                int centre = effect->section.sectionSize % 2 == 0
                             ? effect->section.sectionSize / 2
                             : (effect->section.sectionSize + 1) / 2;

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

            if (effect->mirror != MIRROR_NONE) {
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    effectBufferArray[effect->section.sectionSize + i] = effectBufferArray[effect->section.end - i];
                }
                if (modifier != nullptr) {
                    for (int i = 0; i < effect->section.sectionSize; i++) {
                        modifierBufferArray[effect->section.sectionSize + i] = modifierBufferArray[effect->section.end -
                                                                                                   i];
                    }
                }
            }

            for (int i = 0; i < effectPixelSections.size(); i++) {
                auto pixelSection = effectPixelSections.at(i);
                for (int j = pixelSection.start; j <= pixelSection.end; j++) {
                    targetArray[j] = (modifier == nullptr) ? effectBufferArray[i]
                                                           : modifierBufferArray[i];
                }
            }
        }
    }
}

void Cluster::clearModifier() {
    FastLED.clear();
    clearEffectOrModifier(effectPerSectionPixels);
    clearEffectOrModifier(modifierPerSectionPixels);
}

void Cluster::clearEffectOrModifier(std::vector<std::pair<Effect *, std::vector<Section>>> &effectMap) {
    for (const auto &item: effectMap) {
        delete item.first;
    }
    effectMap.clear();
}