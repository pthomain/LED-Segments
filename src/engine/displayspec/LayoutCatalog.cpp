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
#include "engine/transitions/none/NoTransition.h"
#include "engine/utils/Utils.h"

template<typename T>
RandomEffect<T> LayoutCatalog::randomEntry(
    const EffectSelector<T> &effectSelector,
    const EffectFactory<T> &defaultValue
) const {
    // Select a random layout
    auto iterator = _uniqueLayouts.begin();
    std::advance(iterator, random8(_uniqueLayouts.size()));
    const uint16_t randomLayoutIndex = *iterator;

    // Select effects and mirrors for the selected layout
    const auto [effects, effectMirrorSelector] = effectSelector(randomLayoutIndex);

    // Calculate the total weigEht of all effects for that layout
    uint16_t totalEffectWeight = 0;
    for (const auto &[_, effectWeight]: effects) {
        totalEffectWeight += effectWeight;
    }

    // Pick random values for effect
    uint16_t randomEffectValue = random16(totalEffectWeight);

    //Find effect that matches the randomEffectValue
    for (const auto &[effectFactory, effectWeight]: effects) {
        // We found a matching effect
        if (randomEffectValue < effectWeight) {
            std::map<Mirror, uint8_t> effectMirrors = effectMirrorSelector(*effectFactory);

            // For the selected effect, calculate the associated mirror weights
            uint16_t totalMirrorWeight = 0;
            for (const auto &[_, mirrorWeight]: effectMirrors) {
                totalMirrorWeight += mirrorWeight;
            }

            uint16_t randomMirrorValue = random16(totalMirrorWeight);

            //Find mirror that matches the randomMirrorValue
            for (const auto &[mirror, mirrorWeight]: effectMirrors) {
                // We found a matching effect, return layout index, effect and mirror
                if (randomMirrorValue < mirrorWeight) {
                    return RandomEffect<T>(randomLayoutIndex, *effectFactory, mirror);
                }

                randomMirrorValue -= mirrorWeight;
            }

            // If no mirror was found, return the default value
            if constexpr (IS_DEBUG) {
                Serial.print("Could not find a valid mirror with value ");
                Serial.print(randomMirrorValue);
                Serial.print(" for layout ");
                Serial.println(layoutName(randomLayoutIndex));
            }
            return {randomLayoutIndex, *effectFactory, Mirror::NONE};
        }

        randomEffectValue -= effectWeight;
    }

    // If no effect was found, return the default value
    if constexpr (IS_DEBUG) {
        Serial.print("Could not find a valid effect with value ");
        Serial.print(randomEffectValue);
        Serial.print(" for layout ");
        Serial.println(layoutName(randomLayoutIndex));
    }

    return RandomEffect<T>{0, defaultValue, Mirror::NONE};
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
