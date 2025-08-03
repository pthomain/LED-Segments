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

#ifndef PHRASE_LAYOUT_CONFIG_H
#define PHRASE_LAYOUT_CONFIG_H

#include "engine/displayspec/LayoutConfig.h"

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

static const std::map<uint16_t, String> phraseLayoutNames = {
    {LEDS_IN_LETTERS, "LEDS_IN_LETTERS"},
    {LEDS_IN_WORDS, "LEDS_IN_WORDS"},
    {LEDS_IN_WHOLE, "LEDS_IN_WHOLE"},
    {LETTERS_IN_WORDS, "LETTERS_IN_WORDS"},
    {LETTERS_IN_WHOLE, "LETTERS_IN_WHOLE"},
    {WORDS_IN_WHOLE, "WORDS_IN_WHOLE"},
};

static WeightedLayouts phraseLayoutSelector(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 0},
                {LEDS_IN_LETTERS, 8},
                {LETTERS_IN_WHOLE, 12},
                {LETTERS_IN_WORDS, 0},
                {WORDS_IN_WHOLE, 8}
            };

        case EffectType::TRANSITION:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 1},
                {LEDS_IN_LETTERS, 4},
                {LETTERS_IN_WHOLE, 8},
                {LETTERS_IN_WORDS, 2},
                {WORDS_IN_WHOLE, 0}
            };

        case EffectType::OVERLAY:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 2},
                {LEDS_IN_LETTERS, 4},
                {LETTERS_IN_WHOLE, 2},
                {LETTERS_IN_WORDS, 2},
                {WORDS_IN_WHOLE, 0}
            };
    }
}

#endif //PHRASE_LAYOUT_CONFIG_H
