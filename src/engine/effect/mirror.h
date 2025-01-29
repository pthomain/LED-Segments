#pragma once

#ifndef LED_SEGMENTS_MIRROR_H
#define LED_SEGMENTS_MIRROR_H

#include <vector>
#include <string>
#include "FastLED.h"

typedef enum {
    MIRROR_NONE,
    MIRROR_CENTRE,
    MIRROR_EDGE
} Mirror;

const std::vector<Mirror> ALL_MIRRORS = {
        MIRROR_NONE,
        MIRROR_CENTRE,
        MIRROR_EDGE,
};


uint16_t getMirrorSize(Mirror mirror, uint16_t effectArraySize);

void applyMirror(Mirror mirror, CRGB *effectArray, uint16_t effectArraySize);

std::string mirrorToString(Mirror mirror);

#endif //LED_SEGMENTS_MIRROR_H
