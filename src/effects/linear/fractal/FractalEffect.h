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

#ifndef LED_SEGMENTS_FRACTALEFFECT_H
#define LED_SEGMENTS_FRACTALEFFECT_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"

namespace LEDSegments {

    class FractalEffect : public Effect<FractalEffect> {
    public:
        static const uint8_t PARAM_SPEED = 0;
        static const uint8_t PARAM_ROUGHNESS = 1;
        const uint16_t speed;
        const uint16_t roughness;

        explicit FractalEffect(const RenderableContext &context)
            : Effect(context),
              speed(param(PARAM_SPEED)),
              roughness(param(PARAM_ROUGHNESS)) {
        }

        void fillSegmentArray(
            CRGB *segmentArray,
            uint16_t segmentArraySize,
            uint16_t segmentIndex,
            fract16 progress,
            unsigned long timeInMillis
        ) override;

        static constexpr const char *name() { return "FractalEffect"; }
        static RenderableFactoryRef<CRGB> factory;
    };

    class FractalEffectFactory : public RenderableFactory<FractalEffectFactory, FractalEffect, CRGB> {
    public:
        static Params declareParams() {
            return {
                    {FractalEffect::PARAM_SPEED, 30},
                    {FractalEffect::PARAM_ROUGHNESS, 140},
                };
        }
    };

} // LEDSegments

#endif //LED_SEGMENTS_FRACTALEFFECT_H
