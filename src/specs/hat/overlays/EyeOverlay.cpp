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
#include <algorithm>

static const EyeOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> EyeOverlay::factory = &factoryInstance;

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
    // {C, CL, L, TL, T, TR, R, CR, C},
    // {C, CR, R, TR, T, TL, L, CL, C},

    // {C, CL, L, BL, B, BR, R, CR, C},
    // {C, CR, R, BR, B, BL, L, CL, C},

    //Eye dashes LTR
    {C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C},
    {C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C, CL, L, L, L, L, L, CL, C, CR, R, R, R, R, R, CR, C},

    //Eye dashes LTR + linger
    {C, CL, L, L, L, L, L, L, L, L, L, L, CL, C, CR, R, R, R, R, R, R, R, R, R, R, CR, C},

    //Eye dashes TB
    {C, T, T, T, T, T, T, T, T, T, T, C, B, B, B, B, B, B, B, B, B, B, C},
    // {C, T, C, B, C, T, C, B, C},
};

// Blink sequence states (pixels to turn black)
static const std::vector<uint8_t> blink_state_1 = {
    0, 1, 2, 3, // Row 0
    33, 34, 35, 36 // Row 6
};
static const std::vector<uint8_t> blink_state_2 = {
    0, 1, 2, 3, // Row 0
    4, 5, 6, 7, 8, // Row 1
    28, 29, 30, 31, 32, // Row 5
    33, 34, 35, 36 // Row 6
};
static const std::vector<uint8_t> blink_state_3 = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36
};

static const std::vector blink_sequence = {
    blink_state_1, // State 1: Start closing
    blink_state_2, // State 2
    blink_state_3, // State 3: Fully closed
    blink_state_2, // State 4
    blink_state_1, // State 5
};

const std::vector<uint8_t> &EyeOverlay::pupilPosition(uint8_t position) {
    return positions.at(position);
}

void EyeOverlay::drawPupil(
    CRGB *effectArray,
    uint16_t effectArraySize,
    unsigned long timeElapsedInMillis
) {
    // --- Blink timing and state update ---
    if (!m_isBlinking) {
        if (timeElapsedInMillis - m_lastBlinkTime > m_nextBlinkInterval) {
            m_isBlinking = true;
            m_lastBlinkTime = timeElapsedInMillis;
            m_blinkStateIndex = 1; // Start from first blink state (partially closed)
        }
    } else {
        const unsigned long blink_duration_per_state = 50; // Adjust as needed
        if (timeElapsedInMillis - m_lastBlinkTime > m_blinkStateIndex * blink_duration_per_state) {
            m_blinkStateIndex++;
            if (m_blinkStateIndex >= blink_sequence.size()) {
                m_isBlinking = false; // Blink finished
                m_blinkStateIndex = 0; // Reset for next blink cycle
                m_lastBlinkTime = timeElapsedInMillis; // Reset blink timer
                m_nextBlinkInterval = random(7000, 10000); // Set new random blink interval
            }
        }
    }

    // --- Draw base eye (dimming) ---
    for (int i = 0; i < effectArraySize; i++) {
        effectArray[i] = eyeDimming;
    }

    // Update sequence every 3 seconds
    if (timeElapsedInMillis - m_lastSequenceChange > m_nextSequenceInterval) {
        m_lastSequenceChange = timeElapsedInMillis;
        m_previousSequenceIndex = m_sequenceIndex;

        uint8_t newSequenceIndex;
        do {
            newSequenceIndex = random8(sequences.size());
        } while (newSequenceIndex == m_previousSequenceIndex);
        m_sequenceIndex = newSequenceIndex;

        m_positionInSequence = 0;
        m_nextSequenceInterval = random(4000, 8000); // Set new random sequence interval
    }

    // Update position in sequence every 100ms
    const auto &currentSequence = sequences[m_sequenceIndex];
    const unsigned long timeInSequence = timeElapsedInMillis - m_lastSequenceChange;
    m_positionInSequence = std::min((unsigned long) (timeInSequence / 80),
                                    (unsigned long) (currentSequence.size() - 1));

    const auto pupilPosEnum = currentSequence[m_positionInSequence];
    for (auto i: pupilPosition(pupilPosEnum)) {
        effectArray[i] = CRGB::Black;
    }

    // --- Apply blink overlay ---
    if (m_isBlinking) {
        for (auto pixel_index: blink_sequence[m_blinkStateIndex]) {
            effectArray[pixel_index] = CRGB::Black;
        }
    }
}

void EyeOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    if (segmentIndex == 0) {
        drawPupil(effectArray, effectArraySize, timeElapsedInMillis);
    } else {
        memset(effectArray, CRGB::White, effectArraySize * sizeof(CRGB));
    }
}
