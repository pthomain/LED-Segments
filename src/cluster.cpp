#include "cluster.h"
#include "utils.h"
#include <utility>
#include <vector>
#include <FastLED.h>
#include <effects/effects.h>

Cluster::Cluster(std::vector<Section>* sections, Scope scope)
    : sections(sections), scope(scope) {
    for (const auto& section: *sections) {
        sectionArrays.emplace_back(new CRGB[section.sectionSize], section);
    }
}

void Cluster::applyTransformation(
    Effect& effect,
    CRGB* targetArray,
    int targetArraySize,
    std::vector<Section>* pixelSections
) {
    if (pixelSections == nullptr) {
        effect.fillArray(targetArray, 0, targetArraySize - 1);
    } else {
        effect.fillArray(targetArray, 0, pixelSections->size() - 1);

        //Starting from the end to keep the temp values in targetArray
        for (int x = pixelSections->size() - 1; x >= 0; --x) {
            Section pixelSection = pixelSections->at(x);
            for (int y = pixelSection.start; y <= pixelSection.end; ++y) {
                targetArray[y] = targetArray[x];
            }
        }
    }
}
