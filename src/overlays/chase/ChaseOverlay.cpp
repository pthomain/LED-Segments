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

#include "ChaseOverlay.h"
#include "crgb.h"
#include "engine/utils/Utils.h"

EffectFactory<CRGB> ChaseOverlay::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<ChaseOverlay>(effectContext);
};

void ChaseOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    return; //TODO fix this
    unsigned long refreshInMillis = 1000 / speed;
    auto lastSparkForSegment = timeElapsedInMillis - lastSparkTimeMillisForSegment[segmentIndex];

    memset(forward, false, sizeof(bool) * context.maxSegmentSize);
    memset(backward, false, sizeof(bool) * context.maxSegmentSize);
    memset(tempForward, false, sizeof(bool) * context.maxSegmentSize);
    memset(tempBackward, false, sizeof(bool) * context.maxSegmentSize);

    auto maxSparks = max(1, static_cast<uint16_t>(effectArraySize * sparkDensity));

    for (int i = 0; i < effectArraySize; i++) {
        if (forward[i]) {
            if (i == effectArraySize - 1) {
                tempBackward[i] = true;
            } else {
                tempForward[i + 1] = true;
            }
            tempForward[i] = false;
        }
        if (backward[i]) {
            if (i == 0) {
                tempForward[i] = true;
            } else {
                tempBackward[i - 1] = true;
            }
            tempBackward[i] = false;
        }
    }

    memcpy(forward, tempForward, sizeof(forward));
    memcpy(backward, tempBackward, sizeof(backward));

    uint16_t sparkCount = 0;
    for (uint16_t position = 0; position < effectArraySize; position++) {
        if (forward[position] || backward[position]) {
            effectArray[position] = CRGB::White;
            sparkCount++;
        }
    }

    if (segmentIndex == 0) {
        Serial.print("sparkCount ");
        Serial.println(sparkCount);
    }

    if (lastSparkForSegment >= refreshInMillis && sparkCount < maxSparks) {
        lastSparkTimeMillisForSegment[segmentIndex] = timeElapsedInMillis;
        forward[0] = true;
    }
}
