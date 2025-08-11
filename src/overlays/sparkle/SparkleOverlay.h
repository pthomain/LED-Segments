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
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Weights.h"

class SparkleOverlay : public Effect<SparkleOverlay, CRGB> {
    const float density;

public:
    static const uint16_t PARAM_DENSITY = 0;

    explicit SparkleOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          density(min(100, param(PARAM_DENSITY)) / 100.0f) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "SparkleOverlay"; }

    WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_SCREEN, 1},
            {EffectOperation::OVERLAY_MULTIPLY, 1},
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class SparkleOverlayFactory : public EffectFactory<SparkleOverlayFactory, SparkleOverlay, CRGB> {
public:
    static Params declareParams() {
        return {
            {SparkleOverlay::PARAM_DENSITY, 50} // 0-100
        };
    }
};

#endif //SPARKLEOVERLAY_H
