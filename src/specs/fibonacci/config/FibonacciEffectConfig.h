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

#ifndef FIBONACCI_EFFECT_CONFIG_H
#define FIBONACCI_EFFECT_CONFIG_H

#include <lib/effects/noise/NoiseEffect.h>
#include <lib/effects/gradient/GradientEffect.h>
#include <lib/effects/swirl/SwirlEffect.h>
#include <lib/effects/slide/SlideEffect.h>
#include "lib/engine/displayspec/config/LayoutConfig.h"

static RenderablesAndMirrors<CRGB> fibonacciEffectSelector(uint16_t layoutId) {
    return {
        {
            {GradientEffect::factory, 1},
            {SwirlEffect::factory, 1},
            {NoiseEffect::factory, 1},
            {SlideEffect::factory, 1}
        },
        [](RenderableFactoryRef<CRGB> effectFactory) {
            return WeightedMirrors{
                {Mirror::REPEAT, 1},
                {Mirror::REPEAT_REVERSE, 1},

                {Mirror::OVERLAY_REPEAT_2, 1},
                {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},

                {Mirror::OVERLAY_REPEAT_3, 1},
                {Mirror::OVERLAY_REPEAT_3_REVERSE, 1}
            };
        }
    };
}

#endif //FIBONACCI_EFFECT_CONFIG_H