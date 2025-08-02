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

#ifndef CHASEOVERLAY_H
#define CHASEOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

class ChaseOverlay : public Effect<ChaseOverlay, CRGB> {
    const uint8_t minSparksPerSegment;
    const uint8_t maxSparksPerSegment;
    const uint8_t intervalBetweenSparks;
    const uint8_t trailLength;

    // Whether the sparks bounce back and forth. Otherwise, new sparks get emitted at the start after they exit.
    const bool isBouncy;
    const bool isSwirling;

    const uint8_t sparksPerSegment = random8(minSparksPerSegment, maxSparksPerSegment);
    const uint16_t swirlingInterval;
    const uint16_t swirlDistance;

    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

    uint16_t leadingSparkPosition = 0;
    uint16_t *sparkIntervalCounterPerSegment;
    uint8_t *nbSparksForSegment;

    std::vector<std::vector<bool> > forward;
    std::vector<std::vector<bool> > backward;

    bool *tempForward;
    bool *tempBackward;

public:
    static const uint8_t PARAM_MIN_SPARKS_PER_SEGMENT = 0;
    static const uint8_t PARAM_MAX_SPARKS_PER_SEGMENT = 1;
    static const uint8_t PARAM_INTERVAL_BETWEEN_SPARKS = 2;
    static const uint8_t PARAM_TRAIL_LENGTH = 3;
    static const uint8_t PARAM_CHANCE_OF_BOUNCE = 4;
    static const uint8_t PARAM_CHANCE_OF_SWIRL = 5;
    static const uint8_t PARAM_OPERATION_MULTIPLY_WEIGHT = 6;
    static const uint8_t PARAM_OPERATION_INVERT_WEIGHT = 7;

    explicit ChaseOverlay(const EffectContext &effectContext);

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    ~ChaseOverlay() override {
        delete[] sparkIntervalCounterPerSegment;
        delete[] nbSparksForSegment;
        delete[] tempForward;
        delete[] tempBackward;
    }

    static constexpr const char *name() { return "ChaseOverlay"; }

    WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {EffectOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class ChaseOverlayFactory : public EffectFactory<ChaseOverlayFactory, ChaseOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT, 1},
            {ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT, 5},
            {ChaseOverlay::PARAM_INTERVAL_BETWEEN_SPARKS, 10},
            {ChaseOverlay::PARAM_TRAIL_LENGTH, 3},
            {ChaseOverlay::PARAM_CHANCE_OF_BOUNCE, 75}, // 0 - 100
            {ChaseOverlay::PARAM_CHANCE_OF_SWIRL, 50}, // 0 - 100
            {ChaseOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 4},
            {ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1},
        };
    }
};

#endif //CHASEOVERLAY_H
