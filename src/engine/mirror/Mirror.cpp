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
        case Mirror::CENTRE:
        case Mirror::EDGE:
        case Mirror::REPEAT:
        case Mirror::REPEAT_REVERSE:
            return (effectArraySize / 2) + (effectArraySize % 2 == 0 ? 0 : 1);

        default:
            return effectArraySize;
    }
}

void applyMirror(Mirror mirror, CRGB *effectArray, uint16_t effectArraySize) {
    if (mirror == Mirror::NONE) return;

    uint16_t mirrorSize = getMirrorSize(mirror, effectArraySize);
    uint8_t offset = effectArraySize % 2 == 0 ? 1 : 0;
    uint16_t centre = mirrorSize - 1;

    if (mirror == Mirror::REVERSE) {
        for (uint16_t index = 0; index < mirrorSize / 2; index++) {
            uint16_t reverseIndex = mirrorSize - 1 - index;

            CRGB temp = effectArray[index];
            effectArray[index] = effectArray[reverseIndex];
            effectArray[reverseIndex] = temp;
        }
    } else if (mirror == Mirror::CENTRE) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[centre - index];
        }
    } else if (mirror == Mirror::EDGE) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[index];
        }
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre - index] = effectArray[centre + index + offset];
        }
    } else if (mirror == Mirror::REPEAT) {
        for (uint16_t index = 0; index < mirrorSize; index++) {
            effectArray[centre + index + offset] = effectArray[index];
        }
    } else if (mirror == Mirror::REPEAT_REVERSE) {
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
        case Mirror::NONE:
            return "NONE";
        case Mirror::REVERSE:
            return "REVERSE";
        case Mirror::CENTRE:
            return "CENTRE";
        case Mirror::EDGE:
            return "EDGE";
        case Mirror::REPEAT:
            return "REPEAT";
        case Mirror::REPEAT_REVERSE:
            return "REPEAT_REVERSE";
        default:
            return "UNKNOWN";
    }
}
