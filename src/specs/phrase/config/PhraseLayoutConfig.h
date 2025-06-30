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

#ifndef PHRASELAYOUTCONFIG_H
#define PHRASELAYOUTCONFIG_H

#include "engine/displayspec/LayoutCatalog.h"
#include "engine/effect/Effect.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/slide/SlideEffect.h>
#include "effects/gradient/GradientEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "engine/overlay/none/NoOverlay.h"
#include "engine/transitions/Transition.h"
#include "engine/transitions/fade/FadeTransition.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,

    LETTERS_IN_WORDS,
    LEDS_IN_WORDS,

    WORDS_IN_WHOLE,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static const std::vector<uint16_t> phraseLayouts = std::vector<uint16_t>{
    LEDS_IN_LETTERS,

    LETTERS_IN_WORDS,
    LEDS_IN_WORDS,

    WORDS_IN_WHOLE,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static EffectAndMirrors<CRGB> phraseEffectSelector(uint16_t layoutIndex) {
    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WORDS:
        case LEDS_IN_WORDS:
        case WORDS_IN_WHOLE:
        case LETTERS_IN_WHOLE:
        case LEDS_IN_WHOLE:
        default:
            return {
                {
                    {&GradientEffect::factory, 1},
                    {&SwirlEffect::factory, 1},
                    {&NoiseEffect::factory, 1},
                    {&SlideEffect::factory, 1}
                },
                allCRGBMirrors
            };
    }
};

static EffectAndMirrors<CRGB> phraseOverlaySelector(uint16_t layoutIndex) {
    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
        case LEDS_IN_WORDS:
        case LEDS_IN_WHOLE:
        case LETTERS_IN_WHOLE:
            return {
                {
                    {&MoireOverlay::factory, 1},
                    {&ChaseOverlay::factory, 1},
                    {&DashOverlay::factory, 1},
                    {&NoOverlay::factory, 5},
                },
                allCRGBMirrors
            };

        default: return {};
    }
};

static EffectAndMirrors<uint8_t> phraseTransitionSelector(uint16_t layoutIndex) {
    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
        case LEDS_IN_WORDS:
        case LEDS_IN_WHOLE:
        case LETTERS_IN_WHOLE: return ALL_TRANSITIONS;

        default: return FADE_TRANSITION;
    }
}

static LayoutCatalog phraseLayoutCatalog() {
    return LayoutCatalog(
        phraseLayouts,
        {
            {LEDS_IN_LETTERS, "LEDS_IN_LETTERS"},
            {LEDS_IN_WORDS, "LEDS_IN_WORDS"},
            {LEDS_IN_WHOLE, "LEDS_IN_WHOLE"},
            {LETTERS_IN_WORDS, "LETTERS_IN_WORDS"},
            {LETTERS_IN_WHOLE, "LETTERS_IN_WHOLE"},
            {WORDS_IN_WHOLE, "WORDS_IN_WHOLE"},
        },
        phraseEffectSelector,
        phraseOverlaySelector,
        phraseTransitionSelector
    );
}

#endif //PHRASELAYOUTCONFIG_H
