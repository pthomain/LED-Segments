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

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

template<typename C>
using MirrorSelector = std::function<WeightedMirrors(RenderableFactoryRef<C> renderableFactory)>;

template<typename C>
WeightedMirrors noMirrors(RenderableFactoryRef<C> renderableFactory) {
    return {};
}

template<typename C>
WeightedMirrors undividedMirrors(RenderableFactoryRef<C> renderableFactory) {
    return {
            {Mirror::NONE, 1},
            {Mirror::REVERSE, 1},
        };
}

template<typename C>
WeightedMirrors unrepeatedMirrors(RenderableFactoryRef<C> renderableFactory) {
    return {
            {Mirror::NONE, 1},
            {Mirror::REVERSE, 1},
            {Mirror::CENTRE, 1},
            {Mirror::EDGE, 1}
    };
}

template<typename C>
WeightedMirrors allMirrors(RenderableFactoryRef<C> renderableFactory) {
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
WeightedMirrors noMirrors(RenderableFactoryRef<uint8_t> renderableFactory);

template
WeightedMirrors noMirrors(RenderableFactoryRef<CRGB> renderableFactory);

template
WeightedMirrors undividedMirrors(RenderableFactoryRef<CRGB> renderableFactory);

template
WeightedMirrors undividedMirrors(RenderableFactoryRef<uint8_t> renderableFactory);

template
WeightedMirrors unrepeatedMirrors(RenderableFactoryRef<uint8_t> renderableFactory);

template
WeightedMirrors unrepeatedMirrors(RenderableFactoryRef<CRGB> renderableFactory);

template
WeightedMirrors allMirrors(RenderableFactoryRef<CRGB> renderableFactory);

template
WeightedMirrors allMirrors(RenderableFactoryRef<uint8_t> renderableFactory);

} // namespace LEDSegments

#endif //LED_SEGMENTS_MIRRORUTILS_H
