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

#ifndef DASHOVERLAY_H
#define DASHOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Weights.h"

class DashOverlay : public Effect<DashOverlay, CRGB> {
    uint16_t *headPositionForSegment;
    uint16_t *tailPositionForSegment;
    bool *isReversedForSegment;

    const uint8_t tailSpeed;
    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

public:
    static const uint8_t PARAM_TAIL_SPEED = 0;
    static const uint16_t PARAM_OPERATION_MULTIPLY_WEIGHT = 1;
    static const uint16_t PARAM_OPERATION_INVERT_WEIGHT = 2;

    explicit DashOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          tailSpeed(max(1, param(PARAM_TAIL_SPEED))),
          multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
          invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
          headPositionForSegment(new uint16_t[context.nbSegments]),
          tailPositionForSegment(new uint16_t[context.nbSegments]),
          isReversedForSegment(new bool[context.nbSegments]) {
        memset(headPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
        memset(tailPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
        memset(isReversedForSegment, 0, context.nbSegments * sizeof(bool));
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    ~DashOverlay() override {
        delete[] headPositionForSegment;
        delete[] tailPositionForSegment;
        delete[] isReversedForSegment;
    }

    static constexpr const char *name() { return "DashOverlay"; }

    WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {EffectOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class DashOverlayFactory : public EffectFactory<DashOverlayFactory, DashOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {DashOverlay::PARAM_TAIL_SPEED, random8(1, 5)},
            {DashOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 4},
            {DashOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1}
        };
    }
};

#endif //DASHOVERLAY_H
