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

#include "EyeOverlay.h"

static const EyeOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> EyeOverlay::factory = &factoryInstance;

static const auto positions = std::vector<std::vector<uint8_t> >{
    {11, 12, 17, 18, 19, 24, 25}, //C
    {1, 2, 5, 6, 7, 11, 12}, //T
    {24, 25, 29, 30, 31, 34, 35}, //B

    {0, 1, 4, 5, 6, 10, 11}, //TL
    {2, 3, 6, 7, 8, 12, 13}, //TR

    {23, 24, 28, 29, 30, 33, 34}, //BL
    {25, 26, 30, 31, 32, 35, 36}, //BR

    {9, 10, 15, 16, 17, 22, 23}, //L
    {13, 14, 19, 20, 21, 26, 27}, //R

    {5, 6, 10, 11, 12, 17, 18}, //C-TL
    {6, 7, 11, 12, 13, 18, 19}, //C-TR

    {17, 18, 23, 24, 25, 29, 30}, //C-BL
    {18, 19, 24, 25, 26, 30, 31}, //C-BR

    {10, 11, 16, 17, 18, 23, 24}, //C-L
    {12, 13, 18, 19, 20, 25, 26} //C-R
};

static const auto sequences = std::vector<std::vector<uint8_t> >{
    //Eye dashes LTR
    {C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C},
    {C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C},

    //Eye dashes LTR + linger
    {C, CL, L, L, L, L, L, L, L, L, L, L, CL, C, CR, R, R, R, R, R, R, R, R, R, R, CR, C},

    //Eye dashes TB
    {C, T, T, T, T, T, T, T, T, T, T, C, B, B, B, B, B, B, B, B, B, B, C},
};

static const std::vector<uint8_t> blinkFrame1 = {
    0, 1, 2, 3,
    33, 34, 35, 36
};

static const std::vector<uint8_t> blinkFrame2 = {
    0, 1, 2, 3,
    4, 5, 6, 7, 8,
    28, 29, 30, 31, 32,
    33, 34, 35, 36
};
static const std::vector<uint8_t> blinkFrame3 = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36
};

static const std::vector blinkSequence = {
    blinkFrame1,
    blinkFrame2,
    blinkFrame3,
    blinkFrame2,
    blinkFrame1,
};

void EyeOverlay::drawPupil(
    CRGB *renderableArray,
    uint16_t renderableArraySize,
    unsigned long timeElapsedInMillis
) {
    if (!isBlinking) {
        if (timeElapsedInMillis - lastBlinkTime > nextBlinkInterval) {
            isBlinking = true;
            lastBlinkTime = timeElapsedInMillis;
            blinkFrameIndex = 1;
        }
    } else {
        const unsigned long blinkFrameDurationInMillis = 50;
        if (timeElapsedInMillis - lastBlinkTime > blinkFrameIndex * blinkFrameDurationInMillis) {
            blinkFrameIndex++;

            if (blinkFrameIndex >= blinkSequence.size()) {
                isBlinking = false;
                blinkFrameIndex = 0;
                lastBlinkTime = timeElapsedInMillis;
                nextBlinkInterval = random(7000, 10000);
            }
        }
    }

    for (int i = 0; i < renderableArraySize; i++) {
        renderableArray[i] = blend(renderableArray[i], CRGB::White, eyeBrightness);
    }

    if (timeElapsedInMillis - lastSequenceChange > nextSequenceInterval) {
        lastSequenceChange = timeElapsedInMillis;
        previousSequenceIndex = sequenceIndex;

        uint8_t newSequenceIndex;
        do {
            newSequenceIndex = random8(sequences.size());
        } while (newSequenceIndex == previousSequenceIndex);

        sequenceIndex = newSequenceIndex;
        positionInSequence = 0;
        nextSequenceInterval = random(5000, 8000);
    }

    const auto &currentSequence = sequences[sequenceIndex];
    const unsigned long timeInSequence = timeElapsedInMillis - lastSequenceChange;
    positionInSequence = min(
        timeInSequence / 80,
        static_cast<unsigned long>(currentSequence.size() - 1)
    );

    for (auto i: positions.at(currentSequence[positionInSequence])) {
        renderableArray[i] = CRGB::Black;
    }

    if (isBlinking) {
        for (auto pixel_index: blinkSequence[blinkFrameIndex]) {
            renderableArray[pixel_index] = CRGB::Black;
        }
    }
}

void EyeOverlay::fillArrayInternal(
    CRGB *renderableArray,
    uint16_t renderableArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    if (segmentIndex == 0) {
        drawPupil(renderableArray, renderableArraySize, timeElapsedInMillis);
    } else {
        memset(renderableArray, CRGB::White, renderableArraySize * sizeof(CRGB));
    }
}