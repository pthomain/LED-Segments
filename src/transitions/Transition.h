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
#include "transitions/slide/SlideTransition.h"

enum class Transition {
    NONE,
    FADE,
    SLIDE
};

const EffectAndMirrors<uint8_t> ALL_TRANSITIONS = {
    WeightedEffects<uint8_t>{
        {FadeTransition::factory, 1},
        {SlideTransition::factory, 1}
    },
    allIntMirrors
};

const EffectAndMirrors<uint8_t> FADE_TRANSITION = {
    {
        {FadeTransition::factory, 1}
    },
    noIntMirrors
};

const EffectAndMirrors<uint8_t> NO_TRANSITION = {
    {
        {NoTransition::factory, 1}
    },
    noIntMirrors
};

static String getTransitionName(Transition transition) {
    switch (transition) {
        case Transition::NONE: return "NONE";
        case Transition::FADE: return "FADE";
        case Transition::SLIDE: return "SLIDE";
        default: return "UNKNOWN";
    }
}

static std::map<Transition, EffectFactoryRef<uint8_t> > transitionFactories = {
    {Transition::NONE, NoTransition::factory},
    {Transition::FADE, FadeTransition::factory},
    {Transition::SLIDE, SlideTransition::factory}
};

#endif //LED_SEGMENTS_TRANSITION_H
