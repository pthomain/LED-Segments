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

#include "DashOverlay.h"
#include "crgb.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

const uint8_t DashOverlay::PARAM_TAIL_SPEED;
const uint16_t DashOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint16_t DashOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const DashOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> DashOverlay::factory = &factoryInstance;

void DashOverlay::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    auto &headPosition = headPositionForSegment.get()[segmentIndex];
    auto &tailPosition = tailPositionForSegment.get()[segmentIndex];
    auto &isReversed = isReversedForSegment.get()[segmentIndex];

    const auto lastIndex = segmentSize - 1;

    if (isReversed) {
        const uint16_t headThreshold = tailSpeed == 1 ? 0 : segmentSize / tailSpeed;
        if (headPosition > 0) headPosition--;

        if (headPosition <= headThreshold && tailPosition > 0) {
            tailPosition -= min(tailSpeed, tailPosition);
        }

        if (headPosition == 0 && tailPosition == 0) {
            isReversed = false;
        }
    } else {
        const uint16_t headThreshold = tailSpeed == 1
                                           ? lastIndex
                                           : segmentSize - (segmentSize / tailSpeed) + 1;

        if (headPosition < lastIndex) headPosition++;

        if (headPosition >= headThreshold && tailPosition < lastIndex) {
            tailPosition += min(tailSpeed, lastIndex - tailPosition);
        }

        if (headPosition == lastIndex && tailPosition == lastIndex) {
            isReversed = true;
        }
    }

    for (uint16_t i = min(headPosition, tailPosition); i <= max(headPosition, tailPosition); i++) {
        segmentArray[i] = CRGB::White;
    }
}

} // namespace LEDSegments
