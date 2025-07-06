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
#include "engine/utils/Weights.h"

static const ChaseOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> ChaseOverlay::factory = &factoryInstance;

ChaseOverlay::ChaseOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          sparkIntervalCounterPerSegment(new uint16_t[context.nbSegments]),
          nbSparksForSegment(new uint8_t[context.nbSegments]),
          forward(std::vector<std::vector<bool> >(context.nbSegments)),
          backward(std::vector<std::vector<bool> >(context.nbSegments)),
          tempForward(new bool[context.maxSegmentSize]),
          tempBackward(new bool[context.maxSegmentSize]) {
    memset(tempForward, 0, sizeof(bool) * context.maxSegmentSize);
    memset(tempBackward, 0, sizeof(bool) * context.maxSegmentSize);
    memset(sparkIntervalCounterPerSegment, 0, sizeof(uint16_t) * context.nbSegments);
    memset(nbSparksForSegment, 0, sizeof(uint8_t) * context.nbSegments);

    for (int i = 0; i < context.nbSegments; i++) {
        forward[i].resize(context.maxSegmentSize, false);
        backward[i].resize(context.maxSegmentSize, false);
    }
}

void ChaseOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    if (effectArraySize < 2) {
        Serial.println("ChaseOverlay: effectArraySize < 2, skipping");
        effectArray[0] = CRGB::White;
        return;
    }

    // Clear temp arrays

    memset(tempForward, 0, context.maxSegmentSize * sizeof(bool));
    memset(tempBackward, 0, context.maxSegmentSize * sizeof(bool));

    // Shift previous sparks back and forth

    uint16_t lastIndex = effectArraySize - 1;

    for (int pixelIndex = 0; pixelIndex < effectArraySize; pixelIndex++) {
        if (forward[segmentIndex][pixelIndex]) {
            if (pixelIndex == lastIndex) {
                if (isBouncy) {
                    tempBackward[lastIndex - 1] = true;
                } else {
                    nbSparksForSegment[segmentIndex] = nbSparksForSegment[segmentIndex] - 1;
                }
            } else {
                tempForward[pixelIndex + 1] = true;
            }
        }
    }

    for (int pixelIndex = lastIndex; pixelIndex >= 0; pixelIndex--) {
        if (backward[segmentIndex][pixelIndex]) {
            if (pixelIndex == 0) {
                if (isBouncy) {
                    tempForward[1] = true;
                } else {
                    nbSparksForSegment[segmentIndex] = nbSparksForSegment[segmentIndex] - 1;
                }
            } else {
                tempBackward[pixelIndex - 1] = true;
            }
        }
    }

    // Copy new shifted values to the real arrays

    forward[segmentIndex].assign(tempForward, tempForward + effectArraySize);
    backward[segmentIndex].assign(tempBackward, tempBackward + effectArraySize);

    // Add new spark if needed

    const uint8_t swirlingInterval = isSwirling ? random8(0, context.nbSegments) : context.nbSegments;
    const uint8_t swirlDistance = effectArraySize / swirlingInterval;

    auto distanceFromLeadingSpark = leadingSparkPosition - sparkIntervalCounterPerSegment[segmentIndex];
    bool isSegmentReady = !isSwirling || distanceFromLeadingSpark >= swirlDistance * segmentIndex;

    bool sparkIsOverdue = isSegmentReady && sparkIntervalCounterPerSegment[segmentIndex] % intervalBetweenSparks == 0;

    if (isSegmentReady) {
        sparkIntervalCounterPerSegment[segmentIndex] =
                (sparkIntervalCounterPerSegment[segmentIndex] + 1) % effectArraySize;
    }

    bool hasRemainingSparks = nbSparksForSegment[segmentIndex] < sparksPerSegment;
    if (hasRemainingSparks && isSegmentReady && sparkIsOverdue) {
        forward[segmentIndex][0] = true;
        nbSparksForSegment[segmentIndex] = nbSparksForSegment[segmentIndex] + 1;
    }

    // Update effect arrays with new sparks + update count

    auto getAlpha = [&](uint16_t position, uint16_t trailIndex) {
        auto relativePosition = static_cast<float>(abs(position - trailIndex));
        auto maxPosition = static_cast<float>(trailLength + 1);
        auto alpha = static_cast<uint8_t>(255.0f * (1.0f - (relativePosition / maxPosition)));
        return CRGB{alpha, alpha, alpha};
    };

    for (uint16_t position = 0; position < effectArraySize; position++) {
        if (forward[segmentIndex][position]) {
            for (int trailIndex = position; trailIndex >= 0 && position - trailIndex <= trailLength; trailIndex--) {
                effectArray[trailIndex] = getAlpha(position, trailIndex);
            }
        }

        if (backward[segmentIndex][position]) {
            for (int trailIndex = position; trailIndex < effectArraySize && trailIndex - position <= trailLength;
                 trailIndex++) {
                effectArray[trailIndex] = getAlpha(position, trailIndex);
            }
        }
    }

    if (leadingSparkPosition < context.nbSegments * swirlDistance) leadingSparkPosition++;
}
