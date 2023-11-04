#include "transformations.h"
#include <utility>
#include <vector>
#include <FastLED.h>
#include <effects/effects.h>

void Cluster::fillWholeArray(
        CRGB *targetArray,
        Mirror mirror,
        Direction direction
) {
    int index = 0;
    for (const auto &array: sectionArrays) {
        for (int i = 0; i < array.second; i++) {
            targetArray[index] = array.first[i];
            index++;
        }
    }
}

Cluster::Cluster(std::vector<Section> *sections) {
    for (const auto &section: *sections) {
        auto sectionSize = 1 + section.second - section.first;
        sectionArrays.emplace_back(std::make_pair(new CRGB[sectionSize], sectionSize));
    }
}

void Cluster::applyTransformation(
        Effect &effect,
        CRGB *targetArray,
        Mirror mirror,
        Direction direction
) {
    for (const auto &pair: sectionArrays) {
        int arraySize;

        switch (mirror) {
            case MIRROR_CENTRE:
            case MIRROR_EDGE:
                arraySize = ceil(static_cast<double>(pair.second) / 2);
                break;
            case MIRROR_NONE:
                arraySize = pair.second;
                break;
        }

        if (sectionArrays.size() == 1) {
            effect.nextFrame(targetArray, arraySize);
        } else {
            effect.nextFrame(pair.first, arraySize);
            fillWholeArray(targetArray, mirror, direction);
        }
    }
}