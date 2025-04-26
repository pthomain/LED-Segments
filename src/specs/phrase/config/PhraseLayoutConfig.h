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

#ifndef PHRASELAYOUTCONFIG_H
#define PHRASELAYOUTCONFIG_H

#include <effects/noise/NoiseEffect.h>
#include <effects/gradient/GradientEffect.h>

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,

    LETTERS_IN_WORDS,
    LEDS_IN_WORDS,

    WORDS_IN_WHOLE,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static const std::vector<uint16_t> phraseLayouts = std::vector<uint16_t>{0, 1, 2, 3, 4, 5};

static const std::vector<uint16_t> pixelHeavyLayouts = std::vector<uint16_t>{
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE
};

static const std::vector<uint16_t> balancedLayouts = std::vector<uint16_t>{
    LEDS_IN_LETTERS,
    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,
};

static const std::vector<uint16_t> segmentHeavyLayouts = std::vector<uint16_t>{
    WORDS_IN_WHOLE,
};

static const std::vector<uint16_t> mirrorableLayouts = std::vector<uint16_t>{
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static const std::map<uint16_t, std::vector<EffectFactory> > &phraseEffects() {
    static const auto map = mapLayoutIndex<EffectFactory>(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            return std::vector{
                NoiseEffect::factory,
                // GradientEffect::factory
            };
        }
    );
    return map;
}

static const std::map<uint16_t, std::vector<Mirror> > &phraseMirrors() {
    static const auto map = mapLayoutIndex<Mirror>(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_LETTERS:
                case LEDS_IN_WORDS:
                case LEDS_IN_WHOLE:
                case LETTERS_IN_WHOLE:
                case LETTERS_IN_WORDS: return ALL_UNREPEATED_MIRRORS;

                case WORDS_IN_WHOLE: ;
                default: return std::vector{Mirror::NONE, Mirror::REVERSE};
            }
        }
    );
    return map;
}

static const std::map<uint16_t, std::vector<Transition> > &phraseTransitions() {
    static const auto map = mapLayoutIndex<Transition>(
        phraseLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_LETTERS:
                case LEDS_IN_WORDS:
                case LEDS_IN_WHOLE:
                case LETTERS_IN_WHOLE: return ALL_TRANSITIONS;

                case LETTERS_IN_WORDS:
                case WORDS_IN_WHOLE:
                default: return std::vector{Transition::FADE};
            }
        }
    );
    return map;
}

//Prevents instantiation before setup() is called
inline const LayoutCatalog &phraseLayoutCatalog() {
    static const auto catalog = LayoutCatalog(
        phraseLayouts.size(),
        {
            {LEDS_IN_LETTERS, "LEDS_IN_LETTERS"},
            {LEDS_IN_WORDS, "LEDS_IN_WORDS"},
            {LEDS_IN_WHOLE, "LEDS_IN_WHOLE"},
            {LETTERS_IN_WORDS, "LETTERS_IN_WORDS"},
            {LETTERS_IN_WHOLE, "LETTERS_IN_WHOLE"},
            {WORDS_IN_WHOLE, "WORDS_IN_WHOLE"},
        },
        phraseEffects(),
        phraseMirrors(),
        phraseTransitions()
    );
    return catalog;
}

#endif //PHRASELAYOUTCONFIG_H
