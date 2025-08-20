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

#ifndef LED_SEGMENTS_WAVEOVERLAY_H
#define LED_SEGMENTS_WAVEOVERLAY_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

class WaveOverlay : public Overlay<WaveOverlay> {
    const bool isClockwise = probability(0.5f);
    const uint16_t scrollingSpeed = random8(2, 4) * 64;
    const uint8_t bpm = random(1, 5) * 5;
    const uint8_t nbWaves = random8(1, 8);
    const uint16_t multiplyOperationWeight;
    const uint16_t invertOperationWeight;

    uint16_t phase = 0;

public:
    static const uint16_t PARAM_OPERATION_MULTIPLY_WEIGHT = 0;
    static const uint16_t PARAM_OPERATION_INVERT_WEIGHT = 1;

    explicit WaveOverlay(const RenderableContext &context)
        : Overlay(context),
          multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
          invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)) {
    }

    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "WaveOverlay"; }

    WeightedOperations operations() {
        return {
            {RenderableOperation::OVERLAY_MULTIPLY, multiplyOperationWeight},
            {RenderableOperation::OVERLAY_INVERT, invertOperationWeight}
        };
    }

    static RenderableFactoryRef<CRGB> factory;
};

class WaveOverlayFactory : public RenderableFactory<WaveOverlayFactory, WaveOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {WaveOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT, 4},
            {WaveOverlay::PARAM_OPERATION_INVERT_WEIGHT, 1}
        };
    }
};

} // namespace LEDSegments

#endif // LED_SEGMENTS_WAVEOVERLAY_H
