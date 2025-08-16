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

#include "engine/render/renderable/TypedRenderable.h"
#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

class MatrixOverlay : public Overlay<MatrixOverlay> {
    const uint8_t densityIncrement;
    const uint8_t minDensity;
    const uint8_t maxDensity;

    const float chanceOfNewStream;
    const uint8_t streamLength;

    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

    struct Stream {
        int16_t position;
        uint8_t length;
        float progress;
        uint16_t duration;
        uint16_t startTime;
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

    explicit MatrixOverlay(const RenderableContext &context);

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    ~MatrixOverlay() override = default;

    static constexpr const char *name() { return "MatrixOverlay"; }

    WeightedOperations operations() {
        return {
            {RenderableOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {RenderableOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static RenderableFactoryRef<CRGB> factory;
};

class MatrixOverlayFactory : public RenderableFactory<MatrixOverlayFactory, MatrixOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {MatrixOverlay::PARAM_MIN_DENSITY, 15}, //0-100
            {MatrixOverlay::PARAM_MAX_DENSITY, 35}, //0-100
            {MatrixOverlay::PARAM_DENSITY_INCREMENT, 5}, //0-10
            {MatrixOverlay::PARAM_STREAM_LENGTH, 6},
            {MatrixOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 6},
            {MatrixOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1},
        };
    }
};

#endif //MATRIXOVERLAY_H
