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

#include "LayoutCatalog.h"
#include <engine/effect/none/NoEffect.h>
#include <engine/overlay/none/NoOverlay.h>

#include "effects/gradient/GradientEffect.h"
#include "engine/transitions/none/NoTransition.h"
#include "engine/utils/Utils.h"
#include "overlays/chase/ChaseOverlay.h"

template<typename T>
uint8_t LayoutCatalog::pickRandomWeight(
    const std::vector<std::pair<T, uint8_t> > &weightedItems
) const {
    uint16_t totalWeight = 0;
    for (const auto &[_, weight]: weightedItems) {
        totalWeight += weight;
    }
    return random16(totalWeight);
}

template<typename T>
T LayoutCatalog::pickRandomWeightedItem(
    const std::vector<std::pair<T, uint8_t> > &weightedItems,
    const T &defaultValue
) const {
    uint16_t randomWeight = pickRandomWeight(weightedItems);

    //Find item that matches the random weight
    for (const auto &[item, itemWeight]: weightedItems) {
        // Return matching item
        if (randomWeight < itemWeight) return item;

        //else continue
        randomWeight -= min(randomWeight, itemWeight);
    }

    // If no item was found, return the default value
    return defaultValue;
}

template<typename T>
RandomEffect<T> LayoutCatalog::randomEntry(
    const EffectSelector<T> &effectSelector,
    const EffectFactory<T> &defaultEffectFactory
) const {
    uint16_t randomLayoutIndex = pickRandomWeightedItem(_weightedLayouts, uint16_t{0});

    const auto &[effects, effectMirrorSelector] = effectSelector(randomLayoutIndex);
    uint16_t randomEffectWeight = pickRandomWeight(effects);

    for (const auto &[effectFactory, effectWeight]: effects) {
        if (randomEffectWeight < effectWeight) {
            auto effectMirrors = effectMirrorSelector(*effectFactory);
            auto randomMirror = pickRandomWeightedItem(effectMirrors, Mirror::NONE);

            return RandomEffect<T>{randomLayoutIndex, *effectFactory , randomMirror};
        }
        randomEffectWeight -= min(randomEffectWeight, effectWeight);
    }

    // If no effect was found, return the default value
    return RandomEffect<T>{0, defaultEffectFactory, Mirror::NONE};
}

RandomEffect<CRGB> LayoutCatalog::randomEffect() const {
    return randomEntry(_effects, NoEffect::factory);
}

RandomEffect<uint8_t> LayoutCatalog::randomTransition() const {
    return randomEntry(_transitions, NoTransition::factory);
}

RandomEffect<CRGB> LayoutCatalog::randomOverlay() const {
    return randomEntry(_overlays, NoOverlay::factory);
}
