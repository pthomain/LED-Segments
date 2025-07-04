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

#ifndef WEIGHTS_H
#define WEIGHTS_H
#include <cstdint>
#include <utility>

#include "Utils.h"
#include "engine/effect/BaseEffect.h"

template<typename T>
using WeightedItem = std::pair<T, uint8_t>;

using WeightedLayout = WeightedItem<uint16_t>;
using WeightedLayouts = std::vector<WeightedLayout>;

template<typename T>
using EffectFactoryRef = const EffectFactory<T> *;

template<typename T>
using WeightedEffect = WeightedItem<EffectFactoryRef<T> >;

template<typename T>
using WeightedEffects = std::vector<WeightedEffect<T> >;

using WeightedMirror = WeightedItem<Mirror>;
using WeightedMirrors = std::vector<WeightedMirror>;

template<typename T>
static T pickRandomWeightedItem(
    const std::vector<WeightedItem<T> > &weightedItems,
    T defaultValue
) {
    if (weightedItems.empty()) return defaultValue;

    // Weighted items are shuffled to ensure that items with the same weights have equal chances to be picked.
    // Fisher-Yates shuffle is used since it's more efficient on Arduino.
    std::vector<WeightedItem<T> > shuffledItems = weightedItems;
    for (size_t i = shuffledItems.size() - 1; i > 0; --i) {
        size_t j = random16(i + 1);
        std::swap(shuffledItems[i], shuffledItems[j]);
    }

    // Calculate total weight
    uint16_t totalWeight = 0;
    for (const auto &[_, weight]: shuffledItems) {
        totalWeight += weight;
    }

    // Pick random weight
    // Minimum weight is 1, random16 is exclusive on the upper bound so 1 is added to include it
    auto targetWeight = random16(totalWeight) + 1;

    // Iterate through shuffled list until the compounded weight reaches the target weight
    uint8_t weightCounter = 0;

    for (const auto &[item, weight]: shuffledItems) {
        weightCounter += weight;
        if (weightCounter >= targetWeight) return item;
    }

    // This should never happen
    if constexpr (IS_DEBUG) {
        Serial.print("No item found for targetWeight ");
        Serial.print(targetWeight);
        Serial.print(" totalWeight ");
        Serial.print(totalWeight);
        Serial.println(" returning default value");
    }

    return defaultValue;
}
#endif //WEIGHTS_H
