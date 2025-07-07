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
#include "overlays/none/NoOverlay.h"
#include "transitions/Transition.h"
#include "engine/utils/Weights.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,
    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,
    WORDS_IN_WHOLE
};

static const std::set<uint16_t> phraseLayouts = {
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,
    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,
    WORDS_IN_WHOLE
};

inline WeightedLayouts phraseLayoutSelector(EffectType effectType) {
    return {
        {LEDS_IN_LETTERS, 4},
        {LEDS_IN_WORDS, 2},
        {LEDS_IN_WHOLE, 1},
        {LETTERS_IN_WORDS, 2},
        {LETTERS_IN_WHOLE, 4},
        {WORDS_IN_WHOLE, 3}
    };
}

static EffectAndMirrors<CRGB> phraseEffectSelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WORDS:
        case LEDS_IN_WORDS:
        case WORDS_IN_WHOLE:
        case LETTERS_IN_WHOLE:
        case LEDS_IN_WHOLE:
        default:
            return {
                {
                    {GradientEffect::factory, 1},
                    {SwirlEffect::factory, 1},
                    {NoiseEffect::factory, 1},
                    {SlideEffect::factory, 1}
                },
                allCRGBMirrors
            };
    }
};

static EffectAndMirrors<CRGB> phraseOverlaySelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
        case LEDS_IN_WORDS:
        case LEDS_IN_WHOLE:
        case LETTERS_IN_WHOLE:
            return EffectAndMirrors<CRGB>{
                {
                    {MoireOverlay::factory, 1},
                    {ChaseOverlay::factory, 1},
                    {DashOverlay::factory, 1},
                    {NoOverlay::factory, 5},
                },
                [](EffectFactoryRef<CRGB> overlayFactory) {
                    if (overlayFactory->is<ChaseOverlay>()
                        || overlayFactory->is<MoireOverlay>()) {
                        return WeightedMirrors{}; //No mirrors for these overlays
                    }
                    return allCRGBMirrors(overlayFactory);
                }
            };

        default: return {};
    }
};

static EffectAndMirrors<uint8_t> phraseTransitionSelector(uint16_t layoutId) {
    switch (layoutId) {
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
        phraseLayoutSelector,
        phraseEffectSelector,
        phraseOverlaySelector,
        phraseTransitionSelector
    );
}

#endif //PHRASELAYOUTCONFIG_H
