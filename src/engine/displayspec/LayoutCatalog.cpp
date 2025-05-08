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

template
std::map<uint16_t, std::vector<EffectFactory<CRGB> > > mapLayoutIndex<EffectFactory<CRGB> >(
    const std::vector<uint16_t> &,
    const std::function<std::vector<EffectFactory<CRGB> >(uint16_t)> &
);

template
std::map<uint16_t, std::vector<EffectFactory<uint8_t> > > mapLayoutIndex<EffectFactory<uint8_t> >(
    const std::vector<uint16_t> &,
    const std::function<std::vector<EffectFactory<uint8_t> >(uint16_t)> &
);

template
std::map<uint16_t, std::vector<Mirror> > mapLayoutIndex<Mirror>(
    const std::vector<uint16_t> &,
    const std::function<std::vector<Mirror>(uint16_t)> &
);

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutIndex(
    const std::vector<uint16_t> &layoutIndexes,
    const std::function<std::vector<T>(uint16_t)> &mapper
) {
    std::map<uint16_t, std::vector<T> > map;
    for (const auto layoutIndex: layoutIndexes) {
        map[layoutIndex] = mapper(layoutIndex);
    }
    return map;
}

template<typename T>
std::pair<uint16_t, T> LayoutCatalog::randomLayoutSpecificEntry(
    const String &entryType,
    const std::map<uint16_t, std::vector<T> > &map,
    const std::pair<uint16_t, T> defaultValue
) const {
    if (map.empty()) {
        if constexpr (IS_DEBUG) {
            Serial.print("No entries in ");
            Serial.print(entryType);
            Serial.println(" map");
        }
        return defaultValue;
    }

    auto iterator = map.begin();
    std::advance(iterator, random8(map.size()));

    const uint16_t randomLayoutIndex = iterator->first;
    const std::vector<T> &entries = iterator->second;

    if (entries.empty()) {
        if constexpr (IS_DEBUG) {
            Serial.print("No ");
            Serial.print(entryType);
            Serial.print(" values provided for layout ");
            Serial.println(randomLayoutIndex);
        }
        return defaultValue;
    }

    const T &entry = entries.at(random8(entries.size()));
    return {randomLayoutIndex, entry};
}

template<typename T>
T LayoutCatalog::randomMapEntryForLayout(
    const String &entryType,
    uint16_t layoutIndex,
    const std::map<uint16_t, std::vector<T> > &map,
    T defaultValue
) const {
    if (map.empty() || map.find(layoutIndex) == map.end()) {
        if constexpr (IS_DEBUG) {
            Serial.print("No entries in ");
            Serial.print(entryType);
            Serial.println(" map");
        }
        return defaultValue;
    }

    auto &entry = map.at(layoutIndex);
    if (entry.empty()) {
        if constexpr (IS_DEBUG) {
            Serial.print("No ");
            Serial.print(entryType);
            Serial.println(" values provided for layout ");
            Serial.println(layoutIndex);
        }
        return defaultValue;
    }
    return entry.at(random8(entry.size()));
}

EffectFactory<CRGB> LayoutCatalog::randomEffectFactory(uint16_t layoutIndex) const {
    return randomMapEntryForLayout(EFFECT_ENTRY, layoutIndex, _effects, NoEffect::factory);
}

Mirror LayoutCatalog::randomMirror(uint16_t layoutIndex) const {
    return randomMapEntryForLayout(MIRROR_ENTRY, layoutIndex, _mirrors, Mirror::NONE);
}

std::pair<uint16_t, EffectFactory<uint8_t> > LayoutCatalog::randomTransition() const {
    return randomLayoutSpecificEntry(TRANSITION_ENTRY, _transitions, {0, NoTransition::factory});
}

std::pair<uint16_t, EffectFactory<CRGB> > LayoutCatalog::randomOverlay() const {
    if (probability(probabilityOfOverlay)) {
        return randomLayoutSpecificEntry(OVERLAY_ENTRY, _overlays, {0, NoOverlay::factory});
    }
    return {0, NoOverlay::factory};
}
