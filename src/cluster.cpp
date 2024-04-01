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

void Cluster::applyEffect(
        const std::function<std::unique_ptr<Effect>(Section &)> &effectFactory,
        const Cluster *allPixels
) {
    //The effect applies to each section of the cluster repeatedly
    //The effect applies to a number of pixels in the section
    //If allPixels is not null, we need to subdivide each section into virtual pixels corresponding to
    //the intersection of the subsections in allPixels and the current section

    effectPerSectionPixels.clear();

    for (auto scopeSection: scopeSections) {
        if (allPixels == nullptr) {
            if (scope == SCOPE_WHOLE) {
                std::unique_ptr<Effect> effect = effectFactory(scopeSection);
                effectPerSectionPixels.emplace_back(std::move(effect), emptySections);
            } else {
                //TODO apply effect to each scope section
                for (auto section: scopeSections) {
                    const auto &string = "adding section: " + std::to_string(section.start) + " to " +
                                         std::to_string(section.end);
//                    Serial.println(string.c_str());

                    std::unique_ptr<Effect> effect = effectFactory(section);
                    effectPerSectionPixels.emplace_back(std::move(effect), emptySections);
                }
            }
        } else {
            auto intersectedSections = intersectAllPixelsWithClusterScope(
                    scopeSection,
                    allPixels->scopeSections
            );
            Section pixelSection = Section(0, intersectedSections.size() - 1);
            std::unique_ptr<Effect> effect = effectFactory(pixelSection);
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

//    printNumber("numberOfVirtualPixels", numberOfVirtualPixels);
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
                //TODO fix this
                const auto &string = "filling section: " + std::to_string(effect->section.start) + " to " +
                                     std::to_string(effect->section.end);
//                Serial.println(string.c_str());

                effect->fillArray(bufferArray);
                for (int i = 0; i < effect->section.sectionSize; i++) {
                    targetArray[effect->section.start + i] = bufferArray[i];
                }
            }
        } else {
            effect->fillArray(bufferArray);

            for (int i = 0; i < pixelSections.size(); i++) {
                auto pixelSection = pixelSections.at(i);
                for (int j = pixelSection.start; j <= pixelSection.end; j++) {
                    targetArray[j] = bufferArray[i];
                }
            }
        }
    }
}
