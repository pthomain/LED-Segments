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
#include "lib/engine/mirror/Mirror.h"

template<typename T>
using WeightedItem = std::pair<T, uint8_t>;

using WeightedLayout = WeightedItem<uint16_t>;
using WeightedLayouts = std::vector<WeightedLayout>;

using WeightedMirror = WeightedItem<Mirror>;
using WeightedMirrors = std::vector<WeightedMirror>;

using WeightedOperations = std::vector<WeightedItem<RenderableOperation>>;

template<typename T>
std::vector<WeightedItem<T>> just(T item) {
    return { {item, 1} };
}

template
std::vector<WeightedItem<uint8_t> > just(uint8_t item);

template
std::vector<WeightedItem<CRGB> > just(CRGB item);

template
std::vector<WeightedItem<RenderableOperation> > just(RenderableOperation item);

template<typename T>
static T pickRandomWeightedItem(
    const std::vector<WeightedItem<T> > &weightedItems,
    T defaultValue
) {
    if (weightedItems.empty()) return defaultValue;

    // Calculate total weight
    uint16_t totalWeight = 0;
    for (const auto &[_, weight]: weightedItems) {
        totalWeight += weight;
    }

    // Pick random weight
    // Minimum weight is 1, random16 is exclusive on the upper bound so 1 is added to include it
    auto targetWeight = random16(totalWeight) + 1;

    // Iterate through shuffled list until the compounded weight reaches the target weight
    uint8_t weightCounter = 0;

    for (const auto &[item, weight]: weightedItems) {
        weightCounter += weight;
        if (weightCounter >= targetWeight) return item;
    }

    return defaultValue;
}
#endif //WEIGHTS_H
