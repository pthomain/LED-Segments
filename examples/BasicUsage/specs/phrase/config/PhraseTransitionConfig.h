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

#ifndef PHRASE_TRANSITION_CONFIG_H
#define PHRASE_TRANSITION_CONFIG_H

#include "PhraseLayoutConfig.h"
#include "engine/displayspec/config/LayoutConfig.h"
#include "transitions/Transition.h"

static RenderablesAndMirrors<uint8_t> phraseTransitionSelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WHOLE:
            return {
                just(SlideTransition::factory),
                unrepeatedMirrors<uint8_t>
            };

        case LETTERS_IN_WORDS:
            return {
                just(SlideTransition::factory),
                undividedMirrors<uint8_t>
            };

        case LEDS_IN_WHOLE:
        case LEDS_IN_WORDS:
        case WORDS_IN_WHOLE:
        default:
            return {
                just(FadeTransition::factory),
                noMirrors<uint8_t>
            };
    }
}

#endif //PHRASE_TRANSITION_CONFIG_H
