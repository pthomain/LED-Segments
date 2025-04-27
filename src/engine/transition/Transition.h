//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>
//
// /*
//  * This file is part of LED Segments.
//  *
//  * LED Segments is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * LED Segments is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with LED Segments. If not, see <https://www.gnu.org/licenses/>.
//  *
//

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

#ifndef LED_SEGMENTS_TRANSITION_H
#define LED_SEGMENTS_TRANSITION_H

#include "crgb.h"

enum class Transition {
    NONE,
    FADE,
    SLIDE
};

const std::vector ALL_TRANSITIONS = {
    Transition::FADE,
    Transition::SLIDE
};

static const String getTransitionName(Transition transition) {
    switch (transition) {
        case Transition::NONE: return "NONE";
        case Transition::FADE: return "FADE";
        case Transition::SLIDE: return "SLIDE";
        default: return "UNKNOWN";
    }
}

static void fillTransitionArray(
    const Transition transition,
    CRGB *transitionArray,
    const uint16_t segmentSize,
    const float transitionPercent
) {
    switch (transition) {
        case Transition::SLIDE: {
            uint16_t limit = constrain(round((float)segmentSize * transitionPercent), 0, segmentSize);
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = i < limit ? CRGB::White : CRGB::Black;
            }
        }
        break;

        case Transition::FADE: {
            uint8_t alpha = 255 * transitionPercent;
            CRGB colour = CRGB(alpha, alpha, alpha);
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = colour;
            }
        }
        break;

        case Transition::NONE:
        default: {
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = transitionPercent < 0.5f ? CRGB::Black : CRGB::White;
            }
        }
            break;
    }
}

#endif //LED_SEGMENTS_TRANSITION_H
