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
    EffectType effectType,
    const String &weightType,
    const std::vector<std::pair<T, uint8_t> > &weightedItems
) const {
    uint16_t totalWeight = 0;
    for (const auto &[_, weight]: weightedItems) {
        totalWeight += weight;
    }

    auto result = random16(totalWeight);
    Serial.print(effectTypeName(effectType));
    Serial.print(" ");
    Serial.print(weightType);
    Serial.print(" total: ");
    Serial.print(totalWeight);
    Serial.print(" result: ");
    Serial.println(result);

    return result;
}

template<typename T>
T LayoutCatalog::shuffleAndPickRandomWeightedItem(
    EffectType effectType,
    const String &weightType,
    const std::vector<std::pair<T, uint8_t> > &weightedItems,
    const T &defaultValue
) const {
    uint16_t randomWeight = pickRandomWeight(effectType, weightType, weightedItems);

    // Weighted items are shuffled to ensure that items with the same weights have equal chances to be picked.
    // Fisher-Yates shuffle is used since it's more efficient on Arduino.
    std::vector<std::pair<T, uint8_t> > shuffledItems = weightedItems;
    for (size_t i = shuffledItems.size() - 1; i > 0; --i) {
        size_t j = random16(i + 1);
        std::swap(shuffledItems[i], shuffledItems[j]);
    }

    // Find item that matches the random weight
    for (const auto &[item, itemWeight]: shuffledItems) {
        // Return matching item
        if (randomWeight < itemWeight) return item;

        // else continue
        randomWeight -= min(randomWeight, itemWeight);
    }

    // If no item was found, return the default value
    return defaultValue;
}

template<typename T>
RandomEffect<T> LayoutCatalog::randomEntry(
    EffectType effectType,
    const EffectSelector<T> &effectSelector,
    const EffectFactory<T> &defaultEffectFactory
) const {
    uint16_t randomLayoutIndex = shuffleAndPickRandomWeightedItem(
        effectType,
        "layout",
        _layoutSelector(effectType),
        uint16_t{0}
    );

    if (randomLayoutIndex == UINT16_MAX) {
        Serial.println("NO " + effectTypeName(effectType) + " layout found");
        return RandomEffect<T>{UINT16_MAX, defaultEffectFactory, Mirror::NONE};
    }

    Serial.println(
        effectTypeName(effectType) + " random layout index: " + String(randomLayoutIndex) + " " + layoutName(
            randomLayoutIndex));

    const auto &[effects, effectMirrorSelector] = effectSelector(randomLayoutIndex);
    uint16_t randomEffectWeight = pickRandomWeight(effectType, "effect", effects);

    Serial.println(effectTypeName(effectType) + " random effect weight: " + String(randomEffectWeight));

    for (const auto &[effectFactory, effectWeight]: effects) {
        if (randomEffectWeight < effectWeight) {
            Serial.println(
                "Found " + effectTypeName(effectType) + " for layout index: " + String(randomLayoutIndex) + " " +
                layoutName(
                    randomLayoutIndex));
            auto effectMirrors = effectMirrorSelector(*effectFactory);
            auto randomMirror = shuffleAndPickRandomWeightedItem(effectType, "mirror", effectMirrors, Mirror::NONE);

            return RandomEffect<T>{randomLayoutIndex, *effectFactory, randomMirror};
        }
        randomEffectWeight -= min(randomEffectWeight, effectWeight);
    }

    Serial.println("NO " + effectTypeName(effectType) + " found for layout index: " + layoutName(randomLayoutIndex));
    // If no effect was found, return the default value
    return RandomEffect<T>{0, defaultEffectFactory, Mirror::NONE};
}

RandomEffect<CRGB> LayoutCatalog::randomEffect() const {
    return randomEntry(EffectType::EFFECT, _effects, NoEffect::factory);
}

RandomEffect<uint8_t> LayoutCatalog::randomTransition() const {
    return randomEntry(EffectType::TRANSITION, _transitions, NoTransition::factory);
}

RandomEffect<CRGB> LayoutCatalog::randomOverlay() const {
    return randomEntry(EffectType::OVERLAY, _overlays, NoOverlay::factory);
}
