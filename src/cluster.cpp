#include "cluster.h"
#include "utils.h"
#include <utility>
#include <functional> // Include for std::function
#include <vector>
#include <FastLED.h>
#include <effects/effects.h>
#include <memory>

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
        const std::function<std::unique_ptr<Effect>(Section &, Mirror)> &effectFactory,
        const Cluster *pixelUnits,
        const Mirror mirror
) {
    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If pixelUnits is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in pixelUnits and the current section

    effectPerSectionPixels.clear();

    for (auto scopeSection: scopeSections) {
        if (pixelUnits == nullptr) {
            //If pixelUnits is null, we don't need to apply a mirror
            if (scope == SCOPE_WHOLE) {
                std::unique_ptr<Effect> effect = effectFactory(scopeSection, MIRROR_NONE);
                effectPerSectionPixels.emplace_back(std::move(effect), emptySections);
            } else {
                for (auto section: scopeSections) {
                    std::unique_ptr<Effect> effect = effectFactory(section, MIRROR_NONE);
                    effectPerSectionPixels.emplace_back(std::move(effect), emptySections);
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

            std::unique_ptr<Effect> effect = effectFactory(pixelSection, mirror);
            effectPerSectionPixels.emplace_back(std::move(effect), intersectedSections);
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

        if (pixelSections.empty()) {
            if (scope == SCOPE_WHOLE) {
                effect->fillArray(targetArray);
            } else {
                effect->fillArray(bufferArray);
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    targetArray[effect->section.start + i] = bufferArray[i];
                }
            }
        } else {
            effect->fillArray(bufferArray);

            if (effect->mirror == MIRROR_EDGE) {
                int centre = effect->section.sectionSize % 2 == 0
                             ? effect->section.sectionSize / 2
                             : (effect->section.sectionSize + 1) / 2;

                for (int i = 0; i < centre; i++) {
                    int right = centre + i;
                    int left = centre - 1 - i;

                    CRGB temp = bufferArray[right];
                    bufferArray[right] = bufferArray[left];
                    bufferArray[left] = temp;
                }
            }

            if (effect->mirror != MIRROR_NONE) {
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    bufferArray[effect->section.sectionSize + i] = bufferArray[effect->section.end - i];
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
