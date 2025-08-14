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

#ifndef HAT_EFFECT_CONFIG_H
#define HAT_EFFECT_CONFIG_H

#include "HatLayoutConfig.h"
#include "effects/noise/NoiseEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "effects/slide/SlideEffect.h"
#include "engine/effect/Effect.h"

static EffectAndMirrors<CRGB> hatEffectSelector(uint16_t layoutId) {
    if (layoutId == EYE_LINEAR) {
        return {
            {
                {SwirlEffect::factory, 2},
                {NoiseEffect::factory, 2},
                {SlideEffect::factory, 1},
                {GradientEffect::factory, 1}
            },
            undividedMirrors<CRGB>
        };
    }

    return {
        {
            {NoiseEffect::factory, 1},
            {GradientEffect::factory, 2}
        },
        undividedMirrors<CRGB>
    };
}

#endif //HAT_EFFECT_CONFIG_H
