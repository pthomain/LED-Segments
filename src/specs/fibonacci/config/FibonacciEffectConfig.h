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

#include <effects/noise/NoiseEffect.h>
#include <effects/gradient/GradientEffect.h>
#include <effects/swirl/SwirlEffect.h>
#include <effects/slide/SlideEffect.h>
#include <engine/displayspec/LayoutConfig.h>
#include "FibonacciLayoutDefinitions.h"

static EffectAndMirrors<CRGB> fibonacciEffectSelector(uint16_t layoutId) {
    if (getAlignment(layoutId) == RADIAL) {
        return {
            {
                {GradientEffect::factory, 1},
                // {SwirlEffect::factory, 1},
                // {NoiseEffect::factory, 1},
                // {SlideEffect::factory, 1}
            },
            allMirrors<CRGB>
        };
    } else {
        return {
            {
                {GradientEffect::factory, 1},
                // {NoiseEffect::factory, 1},
            },
            noMirrors<CRGB>
        };
    }
}

#endif //FIBONACCI_EFFECT_CONFIG_H
