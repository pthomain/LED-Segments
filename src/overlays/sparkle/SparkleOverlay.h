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

#ifndef SPARKLEOVERLAY_H
#define SPARKLEOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/utils/Weights.h"

class SparkleOverlay : public Effect<SparkleOverlay, CRGB> {
    const float density;

public:
    static const uint16_t PARAM_DENSITY = 0;

    explicit SparkleOverlay(const RenderableContext &context)
        : Effect(context),
          density(min(100, param(PARAM_DENSITY)) / 100.0f) {
    }

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "SparkleOverlay"; }

    WeightedOperations operations() {
        return {
            {RenderableOperation::OVERLAY_SCREEN, 1},
            {RenderableOperation::OVERLAY_MULTIPLY, 1},
        };
    }

    static RenderableFactoryRef<CRGB> factory;
};

class SparkleOverlayFactory : public RenderableFactory<SparkleOverlayFactory, SparkleOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {SparkleOverlay::PARAM_DENSITY, 50} // 0-100
        };
    }
};

#endif //SPARKLEOVERLAY_H