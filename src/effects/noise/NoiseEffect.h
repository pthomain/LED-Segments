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

#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include "effects/gradient/GradientEffect.h"
#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Weights.h"

class NoiseEffect : public Effect<NoiseEffect> {
protected:
    const uint8_t paletteScale;
    const uint8_t noiseScale;
    const uint8_t noiseSpeed;
    const uint8_t randomStart = random8();
    const uint8_t speedDivider;

public:
    static const uint8_t PARAM_PALETTE_SCALE = 0;
    static const uint8_t PARAM_NOISE_SCALE = 1;
    static const uint8_t PARAM_NOISE_SPEED = 2;
    static const uint8_t PARAM_SPEED_DIVIDER = 3;

    explicit NoiseEffect(const RenderableContext &context): Effect(context),
          paletteScale(param(PARAM_PALETTE_SCALE)),
          noiseScale(param(PARAM_NOISE_SCALE)),
          noiseSpeed(param(PARAM_NOISE_SPEED)),
          speedDivider(max(1, param(PARAM_SPEED_DIVIDER))) {
    }

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "NoiseEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class NoiseEffectFactory : public RenderableFactory<NoiseEffectFactory, NoiseEffect, CRGB> {
public:
    static Params declareParams() {
        return {
            {NoiseEffect::PARAM_PALETTE_SCALE, random8(2, 6)},
            {NoiseEffect::PARAM_NOISE_SCALE, random8(10, 20)},
            {NoiseEffect::PARAM_NOISE_SPEED, random8(5, 100)},
            {NoiseEffect::PARAM_SPEED_DIVIDER, 5}
        };
    }
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
