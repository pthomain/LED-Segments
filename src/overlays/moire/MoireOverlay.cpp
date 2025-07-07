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

#include "MoireOverlay.h"
#include "crgb.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

static const MoireOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> MoireOverlay::factory = &factoryInstance;

void MoireOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = backColour;
    }

    const uint8_t distance = context.nbSegments * headLength;
    const uint8_t start = (isClockwise ? context.nbSegments - segmentIndex : segmentIndex) * headLength;

    auto &headIndex = headPositionForSegment[segmentIndex];

    for (int32_t pixelIndex = 0; pixelIndex < effectArraySize; pixelIndex++) {
        bool isHead = ((pixelIndex + headIndex) % distance) - start == 0;

        if (isHead) {
            for (uint8_t trailIndex = 0; trailIndex < headLength && pixelIndex; trailIndex++) {
                int32_t index = pixelIndex + trailIndex;
                if (index > 0 && index < effectArraySize) {
                    effectArray[index] = frontColour;
                    reverseArray[index] = frontColour;
                }
            }
        }
    }

    headIndex = ++headIndex % distance;
}
