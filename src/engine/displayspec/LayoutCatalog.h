//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

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

#ifndef LAYOUTCATALOG_H
#define LAYOUTCATALOG_H

#include <cstdint>
#include <map>
#include <vector>
#include <engine/effect/Effect.h>
#include <engine/mirror/Mirror.h>
#include <engine/transition/Transition.h>

static const String UNKNOWN = "UNKNOWN";
static const String EFFECT_ENTRY = "effect";
static const String MIRROR_ENTRY = "mirror";

class LayoutCatalog {
    const uint16_t _nbLayouts;
    const std::map<uint16_t, String> _layoutNames{};
    const std::map<uint16_t, std::vector<EffectFactory> > _effects{};
    const std::map<uint16_t, std::vector<Mirror> > _mirrors{};
    const std::map<uint16_t, std::vector<Transition> > _transitions{};

    template<typename T>
    const T &randomMapEntryForLayout(
        const String &entryType,
        uint16_t layoutIndex,
        const std::map<uint16_t, std::vector<T> > &map,
        const T &defaultValue
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

public:
    explicit LayoutCatalog(
        const uint16_t nbLayouts,
        const std::map<uint16_t, String> &layoutNames,
        const std::map<uint16_t, std::vector<EffectFactory> > &effects,
        const std::map<uint16_t, std::vector<Mirror> > &mirrors,
        const std::map<uint16_t, std::vector<Transition> > &transitions
    ) : _nbLayouts(nbLayouts),
        _layoutNames(std::move(layoutNames)),
        _effects(std::move(effects)),
        _mirrors(std::move(mirrors)),
        _transitions(std::move(transitions)) {
    }

    const uint16_t nbLayouts() const {
        return _nbLayouts;
    }

    virtual String layoutName(uint16_t layoutIndex) const {
        return _layoutNames.find(layoutIndex) == _layoutNames.end() ? UNKNOWN : _layoutNames.at(layoutIndex);
    }

    const EffectFactory &randomEffectFactory(uint16_t layoutIndex) const;

    const Mirror randomMirror(uint16_t layoutIndex) const;

    const std::pair<uint16_t, Transition> randomTransition() const;

    virtual ~LayoutCatalog() = default;
};

template<typename T>
const std::map<uint16_t, std::vector<T> > &mapLayoutIndex(
    const std::vector<uint16_t> layoutIndexes,
    const std::function<std::vector<T> (uint16_t)> mapper
) {
    static const std::map<uint16_t, std::vector<T> > map = [layoutIndexes, mapper] {
        auto innerMap = std::map<uint16_t, std::vector<T> >();
        for (const auto layoutIndex: layoutIndexes) {
            innerMap[layoutIndex] = mapper(layoutIndex);
        }
        return innerMap;
    }();
    return map;
}

#endif //LAYOUTCATALOG_H
