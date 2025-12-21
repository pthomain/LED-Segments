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

#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

class NoiseEffect : public Effect<NoiseEffect> {
protected:
    const uint8_t noiseScale;
    const uint8_t randomStart = random8();
    const uint8_t speedDivider;

public:
    static const uint8_t PARAM_NOISE_SCALE = 0;
    static const uint8_t PARAM_SPEED_DIVIDER = 1;

    explicit NoiseEffect(const RenderableContext &context): Effect(context),
          noiseScale(param(PARAM_NOISE_SCALE)),
          speedDivider(max(1, param(PARAM_SPEED_DIVIDER))) {
    }

    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "NoiseEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class NoiseEffectFactory : public RenderableFactory<NoiseEffectFactory, NoiseEffect, CRGB> {
public:
    static Params declareParams() {
        return {
            {NoiseEffect::PARAM_NOISE_SCALE, random8(40, 80)},
            {NoiseEffect::PARAM_SPEED_DIVIDER, random(10, 30)}
        };
    }
};

} // namespace LEDSegments

#endif //LED_SEGMENTS_NOISEEFFECT_H
