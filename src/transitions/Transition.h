//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain

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

#include "engine/mirror/MirrorUtils.h"
#include "transitions/fade/FadeTransition.h"
#include "transitions/none/NoTransition.h"
#include "transitions/linear/slide/SlideTransition.h"

namespace LEDSegments {

enum class TransitionType { NONE, FADE, SLIDE };

const RenderablesAndMirrors<uint8_t> ALL_TRANSITIONS = {
    WeightedRenderables<uint8_t>{
        {FadeTransition::factory, 1},
        {SlideTransition::factory, 1}
    },
    allMirrors<uint8_t>
};

const RenderablesAndMirrors<uint8_t> FADE_TRANSITION = {
    just(FadeTransition::factory),
    noMirrors<uint8_t>
};

const RenderablesAndMirrors<uint8_t> SLIDE_TRANSITION = {
    just(SlideTransition::factory),
    noMirrors<uint8_t>
};

const RenderablesAndMirrors<uint8_t> NO_TRANSITION = {
    just(NoTransition::factory),
    noMirrors<uint8_t>
};

static String getTransitionName(TransitionType transition) {
    switch (transition) {
        case TransitionType::NONE: return "NONE";
        case TransitionType::FADE: return "FADE";
        case TransitionType::SLIDE: return "SLIDE";
        default: return "UNKNOWN";
    }
}

} // namespace LEDSegments

#endif //LED_SEGMENTS_TRANSITION_H