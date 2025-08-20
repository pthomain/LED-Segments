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

#ifndef TEST_EFFECT_CONFIG_H
#define TEST_EFFECT_CONFIG_H

#include "../../customeffect/CustomEffect.h" // CustomEffect is an example of a user-defined effect
#include "LayoutConfig.h"
#include "effects/fractal/FractalEffect.h" //below are library-provided effects
#include "effects/noise/NoiseEffect.h"
#include "effects/slide/SlideEffect.h"
#include "effects/chaos/ChaosEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "effects/rose/RoseEffect.h"
#include "engine/render/renderable/TypedRenderable.h"

// Defines which effects are randomly picked for the given layout.
// Effects only affect pixels' colour (e.g. gradient animations)
static RenderablesAndMirrors<CRGB> effectSelector(uint16_t layoutId) {
    switch (layoutId) {
        default:
            return {
                {
                    {FractalEffect::factory, 4},
                    {RoseEffect::factory, 4},
                    {ChaosEffect::factory, 4},
                    // {SwirlEffect::factory, 4},
                    //the effects being used for GROUP_BY_4 and GROUP_BY_8 (and their weights)
                    // {CustomEffect::factory, 2}, //you can provide your own effects this way
                    // {NoiseEffect::factory, 3},
                    // {SlideEffect::factory, 1}, //TODO fix
                    // {GradientEffect::factory, 1} //TODO fix
                },
                [](RenderableFactoryRef<CRGB> factory) {
                    //you can use the factory parameter to apply specific mirrors to a given effect
                    // if (factory->is<SwirlEffect>()) {
                        return noMirrors(factory);
                    // }

                    return allMirrors<CRGB>(factory); //apply any mirror to all other effects
                }
            };
        //
        // default:
        //     return {
        //         just(SwirlEffect::factory), //only SwirlEffect is used for GROUP_BY_8 and GROUP_BY_16
        //         noMirrors<CRGB> //no mirror applied to SwirlEffect
        //     };
    };
}

#endif // TEST_EFFECT_CONFIG_H
