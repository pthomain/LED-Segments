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

#ifndef LED_SEGMENTS_GRADIENTEFFECT_H
#define LED_SEGMENTS_GRADIENTEFFECT_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Weights.h"

class GradientEffect : public Effect<GradientEffect> {
    const uint8_t colourStart;
    const uint8_t variation8;

public:
    static const uint8_t PARAM_COLOUR_START = 0;
    static const uint8_t PARAM_DENSITY_VARIATION = 1;

    explicit GradientEffect(const RenderableContext &context): Effect(context),
          colourStart(param(PARAM_COLOUR_START)),
          variation8(
              random8(
                  127 - param(PARAM_DENSITY_VARIATION) / 2.0f,
                  127 + param(PARAM_DENSITY_VARIATION) / 2.0f
              ) - 127
          ) {
    }

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeInMillis
    ) override;

    static constexpr const char *name() { return "GradientEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class GradientEffectFactory : public RenderableFactory<GradientEffectFactory, GradientEffect, CRGB> {
public:
    static Params declareParams() {
        return {
            {GradientEffect::PARAM_COLOUR_START, random8()},
            {GradientEffect::PARAM_DENSITY_VARIATION, random8(85)}
        };
    }
};

#endif //LED_SEGMENTS_GRADIENTEFFECT_H
