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

static const DashOverlayFactory factoryInstance;
const EffectFactory<CRGB> &DashOverlay::factory = factoryInstance;

void DashOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    auto &headPosition = headPositionForSegment[segmentIndex];
    auto &tailPosition = tailPositionForSegment[segmentIndex];
    auto &isReversed = isReversedForSegment[segmentIndex];

    const auto lastIndex = effectArraySize - 1;

    if (isReversed) {
        const uint16_t headThreshold = tailSpeed == 1
                                           ? 0
                                           : effectArraySize / tailSpeed;
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
                                           : effectArraySize - (effectArraySize / tailSpeed) + 1;

        if (headPosition < lastIndex) headPosition++;

        if (headPosition >= headThreshold && tailPosition < lastIndex) {
            tailPosition += min(tailSpeed, lastIndex - tailPosition);
        }

        if (headPosition == lastIndex && tailPosition == lastIndex) {
            isReversed = true;
        }
    }

    for (uint16_t i = min(headPosition, tailPosition); i <= max(headPosition, tailPosition); i++) {
        effectArray[i] = CRGB::White;
    }
}
