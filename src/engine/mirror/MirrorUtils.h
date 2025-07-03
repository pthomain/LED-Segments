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

#include <map>

using WeightedMirrors = std::vector<std::pair<Mirror, uint8_t>>;

template<typename C>
using MirrorSelector = std::function<WeightedMirrors(const EffectFactory<C> &effectFactory)>;

inline WeightedMirrors unrepeatedMirrors(const EffectFactory<CRGB> &effectFactory) {
    return {
        {Mirror::NONE, 1},
        {Mirror::REVERSE, 1},
        {Mirror::CENTRE, 1},
        {Mirror::EDGE, 1}
    };
}

inline WeightedMirrors allCRGBMirrors(const EffectFactory<CRGB> &effectFactory) {
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

inline WeightedMirrors allIntMirrors(const EffectFactory<uint8_t> &effectFactory) {
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

#endif //LED_SEGMENTS_MIRRORUTILS_H
