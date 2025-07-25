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

#ifndef LAYOUTCONFIG_H
#define LAYOUTCONFIG_H

#include <map>
#include <set>
#include <utility>
#include <vector>
#include <engine/effect/Effect.h>
#include <engine/mirror/Mirror.h>
#include <engine/mirror/MirrorUtils.h>
#include "engine/utils/Utils.h"

using LayoutSelector = std::function<WeightedLayouts(EffectType effectType)>;

template<typename C>
using EffectAndMirrors = std::pair<WeightedEffects<C>, MirrorSelector<C> >;

template<typename C>
using EffectSelector = std::function<EffectAndMirrors<C>(uint16_t layoutId)>;

using EffectParamSelector = std::function<Params(
    TypeInfo::ID effectId,
    Mirror mirror
)>;

template<typename C>
using RandomEffect = std::tuple<uint16_t, EffectFactoryRef<C>, Mirror>;

class LayoutConfig {
    const std::map<uint16_t, String> _layoutNames;
    const EffectSelector<CRGB> _effects;
    const EffectSelector<CRGB> _overlays;
    const EffectSelector<uint8_t> _transitions;
    const LayoutSelector _layoutSelector;
    const EffectParamSelector _paramSelector;

    template<typename T>
    RandomEffect<T> randomEntry(
        EffectType effectType,
        const EffectSelector<T> &effectSelector,
        EffectFactoryRef<T> defaultEffectFactory
    ) const;

public:
    const std::set<uint16_t> layoutIds;

    explicit LayoutConfig(
        const std::set<uint16_t> &layoutIds,
        const std::map<uint16_t, String> &layoutNames,
        LayoutSelector layoutSelector,
        EffectSelector<CRGB> effects,
        EffectSelector<CRGB> overlays,
        EffectSelector<uint8_t> transitions,
        EffectParamSelector paramSelector
    ): layoutIds(layoutIds),
       _layoutNames(layoutNames),
       _layoutSelector(std::move(layoutSelector)),
       _effects(std::move(effects)),
       _overlays(std::move(overlays)),
       _transitions(std::move(transitions)),
       _paramSelector(std::move(paramSelector)) {
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

    Params params(TypeInfo::ID effectId, Mirror mirror) const;

    virtual
    ~LayoutConfig() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutId(
    const std::vector<uint16_t> &layoutIds,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCONFIG_H
