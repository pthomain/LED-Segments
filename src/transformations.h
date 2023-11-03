#pragma once

#include <utility>
#include <vector>

using Section = std::pair<uint8_t, uint8_t>;

typedef enum {
    LTR = 0,
    RTL = 1,
} Direction;

typedef enum {
    MIRROR_CENTRE = 0,
    MIRROR_EDGE = 1,
    MIRROR_NONE = 2,
} Mirror;

class Cluster {

private:
    std::vector<std::pair<CRGB *, int>> sectionArrays;

    void fillWholeArray(
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

public:

    Cluster(std::vector<Section> *sections) {
        for (const auto &section: *sections) {
            auto sectionSize = 1 + section.second - section.first;
            sectionArrays.emplace_back(std::make_pair(new CRGB[sectionSize], sectionSize));
        }
    }

    void applyTransformation(
            Effect effect,
            CRGB *targetArray,
            Mirror mirror = MIRROR_NONE,
            Direction direction = RTL
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

            effect.nextFrame(pair.first, arraySize);
            fillWholeArray(targetArray, mirror, direction);
        }
    };
};
