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

const String UNKNOWN = "UNKNOWN";

enum class LayoutDescription {
    /* Layouts with a high ratio of pixels to segments:
     * - good for gradients and repeating patterns across segments
     * - should be used by effects that require room to render nicely (animations, complex gradients, etc)
     */
    FINE,

    /* Layouts with a balanced ratio of pixels to segments:
     * - should be used by effects that can scale to a smaller number of pixels and will be repeated across many segments
     * - might squash complex gradients and make animations jerky
     */
    BALANCED,

    /* Layout with a low ratio of pixels to segments:
     * - should be used by effects that provide few colours or simple gradients
     * - gradients will be squashed and only show a subset of contrasted colours in the possible range
     * - animations should not be used for these layouts
     */
    COARSE,

    /* Layouts on which effects can be mirrored:
     * - these usually correlate highly with the FINE layouts with a few exceptions
     * - the amount of pixels is effectively halved for the mirror to be applied, therefore FINE layouts might render
     * in a similar way as BALANCED or COARSE once mirrored so consideration should be given to how complex gradients
     * and animations will be affected.
     * - usually only the finest layouts will fit the bill
     */
    // MIRRORABLE,

    /* Layouts on which transitions can be applied (excluding FADE):
     * - the same considerations as for the COARSE layouts apply, effects might look jerky
     * - usually correlate highly with the FINE and BALANCED layouts, with a few exceptions
     */
    // TRANSITIONABLE,

    /* Layouts that are good for highlights:
     * - these layouts usually correlate highly with the COARSE layouts but some BALANCED ones might apply too
     * - it makes more sense to highlight segments than individual pixels
     * - it might also make sense to select layout where there is no recursion (depth of 1 with only segments)
     */
    HIGHLIGHTABLE
};

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
