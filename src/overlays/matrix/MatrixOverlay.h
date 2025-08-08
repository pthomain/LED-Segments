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

#ifndef MATRIXOVERLAY_H
#define MATRIXOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

class MatrixOverlay : public Effect<MatrixOverlay, CRGB> {
    const uint8_t densityIncrement;
    const uint8_t minDensity;
    const uint8_t maxDensity;

    const float chanceOfNewStream;
    const uint8_t streamLength;

    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

    struct Stream {
        int16_t position;
        uint8_t speed;
        uint8_t length;
        uint8_t lifespan;
        uint8_t age;
        bool isAlive;
    };

    std::vector<std::vector<Stream> > streams;

public:
    static const uint8_t PARAM_MIN_DENSITY = 0;
    static const uint8_t PARAM_MAX_DENSITY = 1;
    static const uint8_t PARAM_DENSITY_INCREMENT = 2;
    static const uint8_t PARAM_STREAM_LENGTH = 3;
    static const uint8_t PARAM_OPERATION_MULTIPLY_WEIGHT = 4;
    static const uint8_t PARAM_OPERATION_INVERT_WEIGHT = 5;

    explicit MatrixOverlay(const EffectContext &effectContext);

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    ~MatrixOverlay() override = default;

    static constexpr const char *name() { return "MatrixOverlay"; }

    WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {EffectOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class MatrixOverlayFactory : public EffectFactory<MatrixOverlayFactory, MatrixOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {MatrixOverlay::PARAM_MIN_DENSITY, 20}, //0-100
            {MatrixOverlay::PARAM_MAX_DENSITY, 40}, //0-100
            {MatrixOverlay::PARAM_DENSITY_INCREMENT, 5}, //0-10
            {MatrixOverlay::PARAM_STREAM_LENGTH, 7},
            {MatrixOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 6},
            {MatrixOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1},
        };
    }
};

#endif //MATRIXOVERLAY_H
