//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

/*
 * This file is part of LED Segments.
 *
 * LED Segments is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LED Segments is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LED Segments. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Mirror.h"

uint16_t getMirrorSize(Mirror mirror, uint16_t effectArraySize) {
    switch (mirror) {
        case MIRROR_CENTRE:
        case MIRROR_EDGE:
        case MIRROR_REPEAT:
        case MIRROR_REPEAT_REVERSE:
            return (effectArraySize / 2) + (effectArraySize % 2 == 0 ? 0 : 1);

        default:
            return effectArraySize;
    }
}

void applyMirror(Mirror mirror, CRGB *effectArray, uint16_t effectArraySize) {
    if (mirror == MIRROR_NONE) return;

    uint16_t mirrorSize = getMirrorSize(mirror, effectArraySize);
    uint8_t offset = effectArraySize % 2 == 0 ? 1 : 0;
    uint16_t centre = mirrorSize - 1;

    if (mirror == MIRROR_REVERSE) {
        for (uint16_t index = 0; index < mirrorSize / 2; index++) {
            uint16_t reverseIndex = mirrorSize - 1 - index;

            CRGB temp = effectArray[index];
            effectArray[index] = effectArray[reverseIndex];
            effectArray[reverseIndex] = temp;
        }
    } else if (mirror == MIRROR_CENTRE) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[centre - index];
        }
    } else if (mirror == MIRROR_EDGE) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[index];
        }
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre - index] = effectArray[centre + index + offset];
        }
    } else if (mirror == MIRROR_REPEAT) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[index];
        }
    } else if (mirror == MIRROR_REPEAT_REVERSE) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            if (index < mirrorSize / 2) {
                uint16_t reverseIndex = mirrorSize - 1 - index;
                CRGB temp = effectArray[index];
                effectArray[index] = effectArray[reverseIndex];
                effectArray[reverseIndex] = temp;
            }
            effectArray[mirrorSize + index] = effectArray[index];
        }
    }
}

String getMirrorName(Mirror mirror) {
    switch (mirror) {
        case MIRROR_NONE:
            return "MIRROR_NONE";
        case MIRROR_REVERSE:
            return "MIRROR_REVERSE";
        case MIRROR_CENTRE:
            return "MIRROR_CENTRE";
        case MIRROR_EDGE:
            return "MIRROR_EDGE";
        case MIRROR_REPEAT:
            return "MIRROR_REPEAT";
        case MIRROR_REPEAT_REVERSE:
            return "MIRROR_REPEAT_REVERSE";
        default:
            return "UNKNOWN_MIRROR";
    }
}
