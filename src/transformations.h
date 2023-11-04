#pragma once

#include <utility>
#include <vector>
#include <FastLED.h>
#include <effects/effects.h>

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
    );

public:

    Cluster(std::vector<Section> *sections);

    void applyTransformation(
            Effect &effect,
            CRGB *targetArray,
            Mirror mirror = MIRROR_NONE,
            Direction direction = RTL
    );
};
