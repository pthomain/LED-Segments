#include "cluster.h"
#include "utils.h"
#include <utility>
#include <functional> // Include for std::function
#include <vector>
#include <FastLED.h>
#include "effects/effect.h"
#include <memory>
#include "modifiers/pongmodifier.h"

Cluster::Cluster(std::vector<Section> sections, Scope scope)
        : scope(scope),
          scopeSections(std::move(sections)),
          clusterSection(
                  Section(
                          scopeSections.at(0).start,
                          scopeSections.at(scopeSections.size() - 1).end
                  )
          ) {}

void Cluster::changeEffect(
        const std::function<Effect *(const Modifier *)> &effectFactory,
        const std::function<Modifier *(const Section &, const Mirror)> &modifierFactory,
        const Cluster *pixelUnits,
        const Mirror mirror
) {
    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If pixelUnits is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in pixelUnits and the current section

    effectPerSectionPixels.clear();

    for (const auto &scopeSection: scopeSections) {
        if (pixelUnits == nullptr) {
            //If pixelUnits is null, we don't need to apply a mirror

            if (scope == SCOPE_WHOLE) {
                Modifier *modifier = modifierFactory(scopeSection, MIRROR_NONE);
                Effect *effect = effectFactory(modifier);
                effectPerSectionPixels.emplace_back(effect, emptySections);
            } else {
                for (auto section: scopeSections) {
                    Modifier *modifier = modifierFactory(section, MIRROR_NONE);
                    Effect *effect = effectFactory(modifier);
                    effectPerSectionPixels.emplace_back(effect, emptySections);
                }
            }
        } else {
            auto intersectedSections = intersectAllPixelsWithClusterScope(
                    scopeSection,
                    pixelUnits->scopeSections
            );

            int intersectedSize = intersectedSections.size();
            int size = mirror == MIRROR_NONE
                       ? intersectedSize
                       : intersectedSize % 2 == 0 ? intersectedSize / 2 : (intersectedSize + 1) / 2;

            Section pixelSection = Section(0, size - 1);

            Modifier *modifier = modifierFactory(pixelSection, mirror);
            Effect *effect = effectFactory(modifier);
            effectPerSectionPixels.emplace_back(effect, intersectedSections);
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
    for (auto &effectPair: effectPerSectionPixels) {
        auto &effect = effectPair.first;
        auto &pixelSections = effectPair.second;
        auto section = effect->modifier->section;
        auto mirror = effect->modifier->mirror;

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
