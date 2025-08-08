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

const uint8_t ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT;
const uint8_t ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT;
const uint8_t ChaseOverlay::PARAM_DISTANCE_BETWEEN_SPARKS;
const uint8_t ChaseOverlay::PARAM_TRAIL_LENGTH;
const uint8_t ChaseOverlay::PARAM_CHANCE_OF_BOUNCE;
const uint8_t ChaseOverlay::PARAM_CHANCE_OF_SWIRL;
const uint8_t ChaseOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint8_t ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const ChaseOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> ChaseOverlay::factory = &factoryInstance;

ChaseOverlay::ChaseOverlay(const EffectContext &effectContext)
    : Effect(effectContext),
      minSparksPerSegment(param(PARAM_MIN_SPARKS_PER_SEGMENT)),
      maxSparksPerSegment(param(PARAM_MAX_SPARKS_PER_SEGMENT)),
      distanceBetweenSparks(param(PARAM_DISTANCE_BETWEEN_SPARKS)),
      trailLength(param(PARAM_TRAIL_LENGTH)),
      isBouncy(probability(param(PARAM_CHANCE_OF_BOUNCE) / 100.0f)),
      swirlDistance(
          probability(param(PARAM_CHANCE_OF_SWIRL) / 100.0f) ? context.maxSegmentSize / context.nbSegments : 0),
      multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
      invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
      sparks(std::vector<std::vector<Spark> >(context.nbSegments)),
      emittedSparkCount(std::vector<uint8_t>(context.nbSegments, 0)) {
}

CRGB ChaseOverlay::getAlpha(int16_t position, int16_t trailIndex) const {
    auto relativePosition = static_cast<float>(abs(position - trailIndex));
    auto alpha = static_cast<uint8_t>(255.0f * (1.0f - (relativePosition / (trailLength + 1.0f))));
    return CRGB{alpha, alpha, alpha};
}

void ChaseOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    auto &segmentSparks = sparks[segmentIndex];

    // Remove sparks that are off-screen
    segmentSparks.erase(
        std::remove_if(
            segmentSparks.begin(),
            segmentSparks.end(),
            [=](const Spark &spark) {
                auto isRemovalNeeded = spark.position >= effectArraySize || spark.isLessThanZero;
                if (isRemovalNeeded) emittedSparkCount[segmentIndex]--;
                return isRemovalNeeded;
            }
        ),
        segmentSparks.end()
    );

    auto startDelay = swirlDistance * segmentIndex;
    auto isStarted = frameCounter >= startDelay;
    uint16_t segmentCounter = frameCounter - startDelay;

    // Add new spark if needed
    if (isStarted
        && segmentCounter % distanceBetweenSparks == 0
        && emittedSparkCount[segmentIndex] < sparksPerSegment) {
        segmentSparks.push_back({0, true});
        emittedSparkCount[segmentIndex]++;
    }

    for (auto &spark: segmentSparks) {
        //Draw spark
        for (int16_t trailIndex = 0; trailIndex <= trailLength; ++trailIndex) {
            int16_t trailPos = spark.position - trailIndex * (spark.isMovingForward ? 1 : -1);
            if (trailPos >= 0 && trailPos < effectArraySize) {
                effectArray[trailPos] = getAlpha(spark.position, trailPos);
            }
        }

        // Move existing spark
        if (spark.isMovingForward) {
            spark.position++;
            spark.isLessThanZero = false;
        } else {
            if (spark.position == 0) {
                spark.isLessThanZero = true;
            } else {
                spark.position--;
            }
        }

        // Bounce spark if necessary
        if (isBouncy) {
            if (spark.position >= effectArraySize) {
                spark.position = effectArraySize - 1;
                spark.isMovingForward = false;
            } else if (spark.isLessThanZero) {
                spark.position = 0;
                spark.isMovingForward = true;
            }
        }
    }
}

void ChaseOverlay::afterFrame(
    float progress,
    unsigned long timeElapsedInMillis
) {
    if (emittedSparkCount[context.nbSegments - 1] == maxSparksPerSegment) {
        frameCounter = 0;
    } else {
        frameCounter++;
    }
}
