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

#ifndef MOIREOVERLAY_H
#define MOIREOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Utils.h"

class MoireOverlay : public Effect<MoireOverlay, CRGB> {
    const bool isClockwise;
    const bool isInverted;

    const CRGB frontColour;
    const CRGB backColour;

    const uint8_t headLength;

    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

    uint16_t *headPositionForSegment;
    CRGB *reverseArray;

public:
    static const uint16_t PARAM_OPERATION_MULTIPLY_WEIGHT = 0;
    static const uint16_t PARAM_OPERATION_INVERT_WEIGHT = 1;
    static const uint16_t PARAM_HEAD_LENGTH = 2;
    static const uint16_t PARAM_IS_CLOCKWISE = 3;
    static const uint16_t PARAM_IS_MASK_INCLUSIVE = 4;

    explicit MoireOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          isClockwise(param(PARAM_IS_CLOCKWISE) > 0),
          isInverted(param(PARAM_IS_MASK_INCLUSIVE) > 0),
          frontColour(isInverted ? CRGB::White : CRGB::Black),
          backColour(isInverted ? CRGB::Black : CRGB::White),
          headLength(param(PARAM_HEAD_LENGTH)),
          multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
          invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
          reverseArray(new CRGB[context.maxSegmentSize]),
          headPositionForSegment(new uint16_t[context.nbSegments]) {
        memset(headPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    // Explicitly deletes the copy constructor for the MoireOverlay class.
    // This prevents objects of this class from being copied, which is important here
    // because the class manages resources (dynamic arrays) that should not be shallow-copied.
    // Attempting to copy a MoireOverlay object will result in a compile-time error.
    MoireOverlay(const MoireOverlay &) = delete;

    ~MoireOverlay() override {
        delete[] headPositionForSegment;
        delete[] reverseArray;
    }

    static constexpr const char *name() { return "MoireOverlay"; }

    WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {EffectOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class MoireOverlayFactory : public EffectFactory<MoireOverlayFactory, MoireOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {MoireOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 4},
            {MoireOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1},
            {MoireOverlay::PARAM_HEAD_LENGTH, 5},
            {MoireOverlay::PARAM_IS_CLOCKWISE, probability(0.5f)},
            {MoireOverlay::PARAM_IS_MASK_INCLUSIVE, probability(0.5f)},
        };
    }
};

#endif //MOIREOVERLAY_H
