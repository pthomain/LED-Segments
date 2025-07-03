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

using WeightedLayout = std::pair<uint16_t, uint8_t>;

using LayoutSelector = std::function<std::vector<WeightedLayout>(EffectType effectType)>;

template<typename C>
using EffectAndMirrors = std::pair<WeightedEffects<C>, MirrorSelector<C> >;

template<typename C>
using EffectSelector = std::function<EffectAndMirrors<C>(uint16_t layoutId)>;

template<typename C>
using RandomEffect = std::tuple<uint16_t, const EffectFactory<C> &, Mirror>;

class LayoutCatalog {
    const std::map<uint16_t, String> _layoutNames;
    const EffectSelector<CRGB> _effects;
    const EffectSelector<CRGB> _overlays;
    const EffectSelector<uint8_t> _transitions;
    const LayoutSelector _layoutSelector;

    template<typename T>
    RandomEffect<T> randomEntry(
        EffectType effectType,
        const EffectSelector<T> &effectSelector,
        const EffectFactory<T> &defaultEffectFactory
    ) const;

    template<typename T>
    uint8_t pickRandomWeight(
        EffectType effectType,
        const String &weightType,
        const std::vector<std::pair<T, uint8_t> > &weightedItems
    ) const;

    template<typename T>
    T shuffleAndPickRandomWeightedItem(
        EffectType effectType,
        const String &weightType,
        const std::vector<std::pair<T, uint8_t> > &weightedItems,
        const T &defaultValue
    ) const;

public:
    const std::set<uint16_t> layoutIds;

    explicit LayoutCatalog(
        const std::set<uint16_t> &layoutIds,
        const std::map<uint16_t, String> &layoutNames,
        LayoutSelector layoutSelector,
        EffectSelector<CRGB> effects,
        EffectSelector<CRGB> overlays,
        EffectSelector<uint8_t> transitions
    ): layoutIds(layoutIds),
       _layoutNames(layoutNames),
       _layoutSelector(std::move(layoutSelector)),
       _effects(std::move(effects)),
       _overlays(std::move(overlays)),
       _transitions(std::move(transitions)) {
    }

    String layoutName(uint16_t layoutId) const {
        if (_layoutNames.find(layoutId) == _layoutNames.end()) {
            return "Layout " + String(layoutId) + " not found";
        }
        return _layoutNames.at(layoutId);
    }

    RandomEffect<CRGB> randomEffect() const;

    RandomEffect<CRGB> randomOverlay() const;

    RandomEffect<uint8_t> randomTransition() const;

    virtual
    ~LayoutCatalog() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutIndex(
    const std::vector<uint16_t> &layoutIdes,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCATALOG_H
