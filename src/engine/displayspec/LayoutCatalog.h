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

using WeightedLayouts = std::pair<uint16_t, uint8_t>;

template<typename C>
using EffectAndMirrors = std::pair<WeightedEffects<C>, MirrorSelector<C> >;

template<typename C>
using EffectSelector = std::function<EffectAndMirrors<C>(uint16_t layoutIndex)>;

template<typename C>
using RandomEffect = std::tuple<uint16_t, const EffectFactory<C> &, Mirror>;

class LayoutCatalog {
    const std::map<uint16_t, String> _layoutNames;

    const EffectSelector<CRGB> _effects;
    const EffectSelector<CRGB> _overlays;
    const EffectSelector<uint8_t> _transitions;
    const std::vector<WeightedLayouts> _weightedLayouts;

    std::vector<uint16_t> _layouts;

    template<typename T>
    RandomEffect<T> randomEntry(
        const EffectSelector<T> &effectSelector,
        const EffectFactory<T> &defaultEffectFactory
    ) const;


public:
    explicit LayoutCatalog(
        const std::vector<WeightedLayouts> &layouts,
        const std::map<uint16_t, String> &layoutNames,
        EffectSelector<CRGB> effects,
        EffectSelector<CRGB> overlays,
        EffectSelector<uint8_t> transitions
    ) : _weightedLayouts(layouts),
        _layoutNames(layoutNames),
        _effects(std::move(effects)),
        _overlays(std::move(overlays)),
        _transitions(std::move(transitions)) {
        for (const auto &[layoutIndex, _]: layouts) {
            _layouts.emplace_back(layoutIndex);
        }
    }

    std::vector<uint16_t> layouts() const {
        return _layouts;
    }

    virtual String layoutName(uint16_t layoutIndex) const {
        if (_layoutNames.find(layoutIndex) == _layoutNames.end()) {
            Serial.println("Layout " + String(layoutIndex) + " not found");
            return UNKNOWN;
        }

        return _layoutNames.at(layoutIndex);
    }

    RandomEffect<CRGB> randomEffect() const;

    RandomEffect<CRGB> randomOverlay() const;

    RandomEffect<uint8_t> randomTransition() const;

    virtual ~LayoutCatalog() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutIndex(
    const std::vector<uint16_t> &layoutIndexes,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCATALOG_H
