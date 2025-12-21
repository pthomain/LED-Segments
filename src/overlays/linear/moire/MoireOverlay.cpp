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

namespace LEDSegments {

const uint16_t MoireOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint16_t MoireOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const MoireOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> MoireOverlay::factory = &factoryInstance;

void MoireOverlay::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    fract16 progress,
    unsigned long timeElapsedInMillis
) {
    for (uint16_t i = 0; i < segmentSize; i++) {
        segmentArray[i] = backColour;
    }

    const uint8_t distance = context.nbSegments * headLength;
    const uint8_t start = (isClockwise ? context.nbSegments - segmentIndex : segmentIndex) * headLength;

    auto &headIndex = headPositionForSegment.get()[segmentIndex];

    for (int32_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
        bool isHead = ((pixelIndex + headIndex) % distance) - start == 0;

        if (isHead) {
            for (uint8_t trailIndex = 0; trailIndex < headLength && pixelIndex; trailIndex++) {
                int32_t index = pixelIndex + trailIndex;
                if (index > 0 && index < segmentSize) {
                    segmentArray[index] = frontColour;
                    reverseArray.get()[index] = frontColour;
                }
            }
        }
    }

    headIndex = (headIndex + 1) % distance;
}

} // namespace LEDSegments
