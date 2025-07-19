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

#pragma once

#ifndef LED_SEGMENTS_MIRRORUTILS_H
#define LED_SEGMENTS_MIRRORUTILS_H

#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Weights.h"

template<typename C>
using MirrorSelector = std::function<WeightedMirrors(EffectFactoryRef<C> effectFactory)>;

template<typename C>
WeightedMirrors noMirrors(EffectFactoryRef<C> effectFactory) {
    return {};
}

template<typename C>
WeightedMirrors undividedMirrors(EffectFactoryRef<C> effectFactory) {
    return {
            {Mirror::NONE, 1},
            {Mirror::REVERSE, 1},
        };
}

template<typename C>
WeightedMirrors unrepeatedMirrors(EffectFactoryRef<C> effectFactory) {
    return {
            {Mirror::NONE, 1},
            {Mirror::REVERSE, 1},
            {Mirror::CENTRE, 1},
            {Mirror::EDGE, 1}
    };
}

template<typename C>
WeightedMirrors allMirrors(EffectFactoryRef<C> effectFactory) {
    return {
            {Mirror::NONE, 1},
            {Mirror::REVERSE, 1},
            {Mirror::CENTRE, 1},
            {Mirror::EDGE, 1},
            //repeated mirrors are useful for circular displays, less so for linear ones
            {Mirror::REPEAT, 1},
            {Mirror::REPEAT_REVERSE, 1}
    };
}


template
WeightedMirrors noMirrors(EffectFactoryRef<uint8_t> effectFactory);

template
WeightedMirrors noMirrors(EffectFactoryRef<CRGB> effectFactory);

template
WeightedMirrors undividedMirrors(EffectFactoryRef<CRGB> effectFactory);

template
WeightedMirrors undividedMirrors(EffectFactoryRef<uint8_t> effectFactory);

template
WeightedMirrors unrepeatedMirrors(EffectFactoryRef<uint8_t> effectFactory);

template
WeightedMirrors unrepeatedMirrors(EffectFactoryRef<CRGB> effectFactory);

template
WeightedMirrors allMirrors(EffectFactoryRef<CRGB> effectFactory);

template
WeightedMirrors allMirrors(EffectFactoryRef<uint8_t> effectFactory);


#endif //LED_SEGMENTS_MIRRORUTILS_H
