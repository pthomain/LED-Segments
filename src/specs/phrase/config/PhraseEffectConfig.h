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

#ifndef PHRASE_EFFECT_CONFIG_H
#define PHRASE_EFFECT_CONFIG_H

#include "engine/displayspec/config/LayoutConfig.h"
#include "PhraseLayoutConfig.h"
#include "engine/effect/Effect.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/slide/SlideEffect.h>
#include "effects/gradient/GradientEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "engine/utils/Weights.h"

static RenderablesAndMirrors<CRGB> phraseRenderableSelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_WHOLE:
        case LEDS_IN_WORDS:
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WHOLE:
            return {
                {
                    {GradientEffect::factory, 2},
                    {SwirlEffect::factory, 8},
                    {NoiseEffect::factory, 4},
                    {SlideEffect::factory, 1}
                },
                [](RenderableFactoryRef<CRGB> effectFactory) {
                    if (effectFactory->is<SlideEffect>()) {
                        return noMirrors(effectFactory);
                    }
                    return WeightedMirrors{
                        {Mirror::NONE, 1},
                        {Mirror::REVERSE, 1},
                        {Mirror::CENTRE, 2},
                        {Mirror::EDGE, 2}
                    };
                }
            };

        case LETTERS_IN_WORDS:
        case WORDS_IN_WHOLE:
        default:
            return {
                {
                    {GradientEffect::factory, 1},
                    {SwirlEffect::factory, 2},
                    {NoiseEffect::factory, 3}
                },
                noMirrors<CRGB>
            };
    }
};

#endif //PHRASE_EFFECT_CONFIG_H