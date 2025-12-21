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

#ifndef LED_SEGMENTS_ROSEEFFECT_H
#define LED_SEGMENTS_ROSEEFFECT_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"

namespace LEDSegments {
    class RoseEffect : public Effect<RoseEffect> {
        const uint8_t rotationSpeed;
        const uint8_t hueSpeed;
        const uint8_t roseNumerator;
        const uint8_t roseDenominator;

    public:
        static const uint8_t PARAM_N_NUMERATOR = 0; // Rose petals numerator
        static const uint8_t PARAM_D_DENOMINATOR = 1; // Rose petals denominator
        static const uint8_t PARAM_ROTATION_SPEED = 2; // Speed of rotation animation
        static const uint8_t PARAM_HUE_SPEED = 3; // Speed of hue cycling

        explicit RoseEffect(const RenderableContext &context);

        void fillSegmentArray(
            CRGB *segmentArray,
            uint16_t segmentArraySize,
            uint16_t segmentIndex,
            fract16 progress,
            unsigned long timeInMillis
        ) override;

        static constexpr const char *name() { return "RoseEffect"; }
        static RenderableFactoryRef<CRGB> factory;

    private:
        void calculateRosePoint(uint8_t angle, uint8_t &x, uint8_t &y);
    };

    class RoseEffectFactory : public RenderableFactory<RoseEffectFactory, RoseEffect, CRGB> {
    public:
        //see https://en.wikipedia.org/wiki/Maurer_rose
        static Params declareParams() {
            return {
                {RoseEffect::PARAM_N_NUMERATOR, random8(1, 11)}, // Numerator
                {RoseEffect::PARAM_D_DENOMINATOR, random8(1, 100)}, // Denominator
                {RoseEffect::PARAM_ROTATION_SPEED, random(3, 6) * 10}, // Moderate rotation
                {RoseEffect::PARAM_HUE_SPEED, random8(2, 4) * 10} // Moderate hue cycling
            };
        }
    };
} // LEDSegments

#endif //LED_SEGMENTS_ROSEEFFECT_H
