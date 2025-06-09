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

#include "PhraseSpec.h"
#include "functional"
#include "engine/utils/Utils.h"

uint16_t PhraseSpec::nbSegments(const uint16_t layoutIndex) const {
    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
            return NB_LETTERS;

        case LEDS_IN_WORDS:
        case LETTERS_IN_WORDS:
            return NB_WORDS;

        default: // WHOLE segment
            return 1;
    }
}

uint16_t PhraseSpec::segmentSize(
    const uint16_t layoutIndex,
    const uint16_t segmentIndex
) const {
    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
            return LETTERS[segmentIndex][1] - LETTERS[segmentIndex][0] + 1;

        case LEDS_IN_WORDS:
            return WORDS[segmentIndex][1] - WORDS[segmentIndex][0] + 1;

        case LEDS_IN_WHOLE:
            return NB_LEDS;

        case LETTERS_IN_WORDS: {
            uint8_t nbLetters = 0;
            auto word = WORDS[segmentIndex];
            for (auto letter: LETTERS) {
                if (letter[0] >= word[0] && letter[1] <= word[1]) nbLetters++;
            }
            return nbLetters;
        }

        case LETTERS_IN_WHOLE:
            return NB_LETTERS;

        case WORDS_IN_WHOLE:
            return NB_WORDS;

        default:
            return 0; //This should not happen
    }
}


void PhraseSpec::mapLeds(
    uint16_t layoutIndex,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    float progress,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    auto mapSingleLed = [&](uint16_t ledIndex, const std::function<void(uint16_t)> &onLedMapped) {
        IS_DEBUG ? mapLedInSnakeDisplay(ledIndex, onLedMapped) : onLedMapped(ledIndex);
    };

    auto mapRange = [&](uint16_t start, uint16_t end) {
        for (uint16_t i = start; i <= end; i++) {
            mapSingleLed(i, onLedMapped);
        }
    };

    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
            mapSingleLed(LETTERS[segmentIndex][0] + pixelIndex, onLedMapped);
            break;

        case LEDS_IN_WORDS:
            mapSingleLed(WORDS[segmentIndex][0] + pixelIndex, onLedMapped);
            break;

        case LEDS_IN_WHOLE:
            mapSingleLed(pixelIndex, onLedMapped);
            break;

        case LETTERS_IN_WORDS: {
            uint16_t intersectingPixelIndex = 0;
            for (uint16_t i = 0; i <= NB_LETTERS; i++) {
                auto letterStart = LETTERS[i][0];
                auto letterEnd = LETTERS[i][1];
                if (letterStart >= WORDS[segmentIndex][0] && letterEnd <= WORDS[segmentIndex][1]) {
                    if (intersectingPixelIndex == pixelIndex) {
                        mapRange(letterStart, letterEnd);
                        return;
                    }
                    intersectingPixelIndex++;
                }
            }
        }
        break;

        case LETTERS_IN_WHOLE:
            mapRange(LETTERS[pixelIndex][0], LETTERS[pixelIndex][1]);
            break;

        case WORDS_IN_WHOLE:
            mapRange(WORDS[pixelIndex][0], WORDS[pixelIndex][1]);
            break;
    };
}
