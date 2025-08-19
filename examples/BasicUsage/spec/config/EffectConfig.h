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
#include "effects/noise/NoiseEffect.h" //below are library-provided effects
#include "effects/slide/SlideEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "engine/render/renderable/TypedRenderable.h"

// Defines which effects are randomly picked for the given layout.
// Effects only affect pixels' colour (e.g. gradient animations)
static RenderablesAndMirrors<CRGB> effectSelector(uint16_t layoutId) {
    switch (layoutId) {
        case GROUP_BY_1:
        case GROUP_BY_2:
        case GROUP_BY_4:
            return RenderablesAndMirrors<CRGB>(
                {
                    {SwirlEffect::factory, 4},  //the effects being used for GROUP_BY_4 and GROUP_BY_8 (and their weights)
                    {CustomEffect::factory, 2}, //you can provide your own effects this way
                    {NoiseEffect::factory, 3},
                    {SlideEffect::factory, 1},
                    {GradientEffect::factory, 1}
                },
                [](RenderableFactoryRef<CRGB> factory) {
                    //you can use the factory parameter to apply specific mirrors to a given effect, see example in OverlaysConfig.h
                    return allMirrors<CRGB>(factory); //apply any mirror to all those effects
                }
            );

        case GROUP_BY_8:
        case GROUP_BY_16:
        default:
            return RenderablesAndMirrors<CRGB>(
                just(SwirlEffect::factory), //only SwirlEffect is used for GROUP_BY_8 and GROUP_BY_16
                [](RenderableFactoryRef<CRGB> factory) {
                    return noMirrors<CRGB>(factory); //no mirror applied to SwirlEffect
                }
            );
    };
}

#endif // TEST_EFFECT_CONFIG_H
