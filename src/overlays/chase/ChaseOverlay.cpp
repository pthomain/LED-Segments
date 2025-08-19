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

namespace LEDSegments {

const uint8_t ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT;
const uint8_t ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT;
const uint8_t ChaseOverlay::PARAM_DISTANCE_BETWEEN_SPARKS;
const uint8_t ChaseOverlay::PARAM_TRAIL_LENGTH;
const uint8_t ChaseOverlay::PARAM_CHANCE_OF_BOUNCE;
const uint8_t ChaseOverlay::PARAM_CHANCE_OF_SWIRL;
const uint8_t ChaseOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint8_t ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const ChaseOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> ChaseOverlay::factory = &factoryInstance;

ChaseOverlay::ChaseOverlay(const RenderableContext &context)
    : Overlay(context),
      minSparksPerSegment(param(PARAM_MIN_SPARKS_PER_SEGMENT)),
      maxSparksPerSegment(param(PARAM_MAX_SPARKS_PER_SEGMENT)),
      distanceBetweenSparks(param(PARAM_DISTANCE_BETWEEN_SPARKS)),
      trailLength(param(PARAM_TRAIL_LENGTH)),
      isBouncy(probability(param(PARAM_CHANCE_OF_BOUNCE) / 100.0f)),
      swirlDistance(probability(param(PARAM_CHANCE_OF_SWIRL) / 100.0f)
                        ? context.maxSegmentSize / context.nbSegments
                        : 0),
      multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
      invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
      sparks(std::vector<std::vector<Spark>>(context.nbSegments)),
      emittedSparkCount(std::vector<uint8_t>(context.nbSegments, 0)) {}

void ChaseOverlay::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
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
                auto isRemovalNeeded = spark.position >= segmentSize || spark.isLessThanZero;
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
            if (trailPos >= 0 && trailPos < segmentSize) {
                segmentArray[trailPos] = CRGB::White;
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
            if (spark.position >= segmentSize) {
                spark.position = segmentSize - 1;
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

} // namespace LEDSegments
