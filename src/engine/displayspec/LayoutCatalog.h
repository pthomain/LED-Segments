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

#ifndef LAYOUTCATALOG_H
#define LAYOUTCATALOG_H

#include <map>
#include <set>
#include <utility>
#include <vector>
#include <engine/effect/Effect.h>
#include <engine/mirror/Mirror.h>
#include <engine/mirror/MirrorUtils.h>
#include "engine/utils/Utils.h"

static const String UNKNOWN = "UNKNOWN";
static const String EFFECT_ENTRY = "effect";
static const String OVERLAY_ENTRY = "overlay";
static const String MIRROR_ENTRY = "mirror";
static const String TRANSITION_ENTRY = "transition";

template<typename E, typename C>
using EffectSelector = std::function<std::pair<WeightedEffects<E, C>, MirrorSelector<E, C> >(uint16_t layoutIndex)>;

class LayoutCatalog {
    const std::vector<uint16_t> _uniqueLayouts;
    const std::map<uint16_t, String> _layoutNames;

    const EffectSelector<CRGB> _effects;
    const EffectSelector<CRGB> _overlays;
    const EffectSelector<uint8_t> _transitions;

    template<typename E, typename T>
    std::tuple<uint16_t, EffectFactory<T>, Mirror> randomEntry(
        const String &entryType,
        const EffectSelector<E, T> &effectSelector,
        const std::tuple<uint16_t, EffectFactory<T>, Mirror> &defaultValue
    ) const;

    template<typename T>
    std::vector<T> &removeDuplicates(std::vector<T> &vec) {
        std::sort(vec.begin(), vec.end());
        auto last = std::unique(vec.begin(), vec.end());
        vec.erase(last, vec.end());
        return vec;
    }

public:
    explicit LayoutCatalog(
        std::vector<uint16_t> uniqueLayouts,
        std::map<uint16_t, String> layoutNames,
        EffectSelector<CRGB> effects,
        EffectSelector<CRGB> overlays,
        EffectSelector<uint8_t> transitions
    ) : _uniqueLayouts(std::move(removeDuplicates(uniqueLayouts))),
        _layoutNames(std::move(layoutNames)),
        _effects(std::move(effects)),
        _overlays(std::move(overlays)),
        _transitions(std::move(transitions)) {
    }

    std::vector<uint16_t> uniqueLayouts() const {
        return _uniqueLayouts;
    }

    virtual String layoutName(uint16_t layoutIndex) const {
        if (_layoutNames.find(layoutIndex) == _layoutNames.end()) {
            Serial.println("Layout " + String(layoutIndex) + " not found");
            return UNKNOWN;
        }

        return _layoutNames.at(layoutIndex);
    }

    std::tuple<uint16_t, EffectFactory<CRGB>, Mirror> randomEffect() const;

    std::tuple<uint16_t, EffectFactory<CRGB>, Mirror> randomOverlay() const;

    std::tuple<uint16_t, EffectFactory<uint8_t>, Mirror> randomTransition() const;

    virtual ~LayoutCatalog() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutIndex(
    const std::vector<uint16_t> &layoutIndexes,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCATALOG_H
