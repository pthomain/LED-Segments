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
#include "engine/overlay/none/NoOverlay.h"
#include "engine/transitions/Transition.h"
#include "engine/transitions/fade/FadeTransition.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "overlays/chase/ChaseOverlay.h"

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

static std::map<uint16_t, std::vector<EffectFactory<CRGB> > > phraseEffects() {
    return mapLayoutIndex<EffectFactory<CRGB> >(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            return std::vector{
                NoiseEffect::factory,
                // GradientEffect::factory,
                // SlideEffect::factory,
            };
        }
    );
}

static std::map<uint16_t, std::vector<EffectFactory<CRGB> > > phraseOverlays() {
    return mapLayoutIndex<EffectFactory<CRGB> >(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_LETTERS:
                case LEDS_IN_WORDS:
                case LEDS_IN_WHOLE: return std::vector{
                        SparkleOverlay::factory,
                        ChaseOverlay::factory,
                    };
                default: return NO_OVERLAYS;
            }
        }
    );
}

static std::map<uint16_t, std::vector<Mirror> > phraseMirrors() {
    return mapLayoutIndex<Mirror>(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_LETTERS:
                case LEDS_IN_WORDS:
                case LEDS_IN_WHOLE:
                case LETTERS_IN_WHOLE:
                case LETTERS_IN_WORDS: return ALL_UNREPEATED_MIRRORS;

                default: return std::vector{Mirror::NONE, Mirror::REVERSE};
            }
        }
    );
}

static std::map<uint16_t, std::vector<EffectFactory<uint8_t> > > phraseTransitions() {
    return mapLayoutIndex<EffectFactory<uint8_t> >(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_LETTERS:
                case LEDS_IN_WORDS:
                case LEDS_IN_WHOLE:
                case LETTERS_IN_WHOLE: return ALL_TRANSITIONS;

                default: return std::vector{FadeTransition::factory};
            }
        }
    );
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
        phraseEffects(),
        phraseOverlays(),
        phraseTransitions(),
        phraseMirrors(),
        0.25f
    );
}

#endif //PHRASELAYOUTCONFIG_H
