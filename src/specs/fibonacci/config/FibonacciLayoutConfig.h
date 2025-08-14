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

#ifndef FIBONACCI_LAYOUT_CONFIG_H
#define FIBONACCI_LAYOUT_CONFIG_H

#include "engine/effect/EffectOperation.h"
#include "engine/utils/Weights.h"
#include "FibonacciLayoutDefinitions.h"

static uint8_t effectLayoutWeight(uint16_t layoutId) {
    return getInflexion(layoutId) == INFLEXION_NONE ? 1 : 0;
}

static uint8_t overlayLayoutWeights(uint16_t layoutId) {
    return getPixelUnit(layoutId) == SEGMENT ? 1 : 0;
}

static uint8_t transitionLayoutWeights(uint16_t layoutId) {
    return 1;
}

static WeightedLayouts fibonacciLayoutSelector(EffectType effectType) {
    auto ids = fibonacciLayoutIds();
    WeightedLayouts result;
    result.reserve(ids.size());

    std::transform(
        ids.begin(),
        ids.end(),
        std::back_inserter(result),
        [&](uint16_t layoutId) {
            switch (effectType) {
                case EffectType::EFFECT:
                    return WeightedLayout{layoutId, effectLayoutWeight(layoutId)};

                case EffectType::OVERLAY:
                    return WeightedLayout{layoutId, overlayLayoutWeights(layoutId)};

                case EffectType::TRANSITION:
                default:
                    return WeightedLayout{layoutId, transitionLayoutWeights(layoutId)};
            }
        }
    );

    return result;
}

#endif //FIBONACCI_LAYOUT_CONFIG_H
