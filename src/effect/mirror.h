#pragma once

#ifndef LED_SEGMENTS_MIRROR_H
#define LED_SEGMENTS_MIRROR_H

#include <vector>
#include <string>

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

std::string mirrorToString(Mirror mirror);

#endif //LED_SEGMENTS_MIRROR_H
