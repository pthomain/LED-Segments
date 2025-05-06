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

uint16_t PhraseSpec::nbPixels(
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

void PhraseSpec::setColour(
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    const uint16_t pixelIndex,
    float progress,
    CRGB *outputArray,
    const CRGB colour
) const {
    auto applyColourToRange = [&](uint16_t start, uint16_t end) {
        for (uint16_t i = start; i <= end; i++) {
            outputArray[i] = colour;
        }
    };

    auto applyColourToPixel = [&](
        uint16_t segmentStart,
        uint16_t segmentEnd,
        uint16_t pixelArraySize,
        const std::function<std::array<uint16_t, 2>(uint16_t)> &getPixel
    ) {
        uint16_t containedPixelIndex = 0;
        for (uint8_t i = 0; i <= pixelArraySize; i++) {
            auto pixel = getPixel(i);
            if (pixel[0] >= segmentStart && pixel[1] <= segmentEnd) {
                if (containedPixelIndex == pixelIndex) {
                    applyColourToRange(pixel[0], pixel[1]);
                    return;
                }
                containedPixelIndex++;
            }
        }
    };

    switch (layoutIndex) {
        case LEDS_IN_LETTERS:
            applyColourToLed(
                LETTERS[segmentIndex][0] + pixelIndex,
                outputArray,
                colour
            );
            break;

        case LEDS_IN_WORDS:
            applyColourToLed(
                WORDS[segmentIndex][0] + pixelIndex,
                outputArray,
                colour
            );
            break;

        case LEDS_IN_WHOLE:
            applyColourToLed(
                pixelIndex,
                outputArray,
                colour
            );
            break;

        case LETTERS_IN_WORDS:
            applyColourToPixel(
                WORDS[segmentIndex][0],
                WORDS[segmentIndex][1],
                NB_LETTERS,
                [&](uint16_t pixelIndex) {
                    return std::array<uint16_t, 2>{LETTERS[pixelIndex][0], LETTERS[pixelIndex][1]};
                }
            );
            break;

        case LETTERS_IN_WHOLE:
            applyColourToRange(LETTERS[pixelIndex][0], LETTERS[pixelIndex][1]);
            break;

        case WORDS_IN_WHOLE:
            applyColourToRange(WORDS[pixelIndex][0], WORDS[pixelIndex][1]);
            break;
    };
}

void PhraseSpec::applyColourToLed(
    const uint16_t ledIndex,
    CRGB *outputArray,
    const CRGB colour
) const {
    const uint8_t ledsPerRow = 8;
    const uint16_t rowIndex = ledIndex / ledsPerRow;
    if (IS_DEBUG && rowIndex % 2 == 1) {
        //handle snake rows
        auto rowStart = rowIndex * ledsPerRow;
        auto rowEnd = rowStart + ledsPerRow - 1;
        auto relativeIndex = ledIndex - rowStart;
        outputArray[rowEnd - relativeIndex] = colour;
    } else {
        outputArray[ledIndex] = colour;
    }
}
