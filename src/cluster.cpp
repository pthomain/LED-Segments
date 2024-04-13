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

void Cluster::applyEffect(
        const EffectConfig *effectConfig
) {
    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If pixelUnits is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in pixelUnits and the current section

//    if (!effectConfig->isModifier) {
//        for (const auto &item: effectPerSectionPixels) {
//            delete item.first;
//        }
//        for (const auto &item: modifierPerSectionPixels) {
//            delete item.first;
//        }
//
//        effectPerSectionPixels.clear();
//        modifierPerSectionPixels.clear();
//    }

    auto &effectMap = effectConfig->isModifier ? modifierPerSectionPixels : effectPerSectionPixels;

    for (const auto &scopeSection: scopeSections) {
        if (effectConfig->pixelUnits == nullptr) {
            //If pixelUnits is null, we don't need to apply a mirror

            if (scope == SCOPE_WHOLE) {
                Effect *effect = effectConfig->effectFactory(scopeSection, MIRROR_NONE);
                effectMap.emplace_back(effect, emptySections);
            } else {
                for (auto section: scopeSections) {
                    Effect *effect = effectConfig->effectFactory(section, MIRROR_NONE);
                    effectMap.emplace_back(effect, emptySections);
                }
            }
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

void Cluster::render(CRGB *targetArray, CRGB *bufferArray) {
    render(targetArray, bufferArray, effectPerSectionPixels);

    if (!modifierPerSectionPixels.empty()) {
        render(targetArray, bufferArray, modifierPerSectionPixels);
    }
}

void Cluster::render(
        CRGB *targetArray,
        CRGB *bufferArray,
        std::vector<std::pair<Effect *, std::vector<Section>>> &effectMap
) {
    for (auto &effectPair: effectMap) {
        auto &effect = effectPair.first;
        auto &pixelSections = effectPair.second;
        auto section = effect->section;
        auto mirror = effect->mirror;

        if (pixelSections.empty()) {
            if (scope == SCOPE_WHOLE) {
                effect->fillArray(targetArray);
            } else {
                effect->fillArray(bufferArray);
                for (int i = 0; i < section.sectionSize; i++) {
                    targetArray[section.start + i] = bufferArray[i];
                }
            }
        } else {
            effect->fillArray(bufferArray);

            if (mirror == MIRROR_EDGE) {
                int centre = section.sectionSize % 2 == 0
                             ? section.sectionSize / 2
                             : (section.sectionSize + 1) / 2;

                for (int i = 0; i < centre; i++) {
                    int right = centre + i;
                    int left = centre - 1 - i;

                    CRGB temp = bufferArray[right];
                    bufferArray[right] = bufferArray[left];
                    bufferArray[left] = temp;
                }
            }

            if (mirror != MIRROR_NONE) {
                for (int i = 0; i < section.sectionSize; i++) {
                    bufferArray[section.sectionSize + i] = bufferArray[section.end - i];
                }
            }
            for (int i = 0; i < pixelSections.size(); i++) {
                auto pixelSection = pixelSections.at(i);
                for (int j = pixelSection.start; j <= pixelSection.end; j++) {
                    targetArray[j] = bufferArray[i];
                }
            }
        }
    }
}
