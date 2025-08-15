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
#include <lib/engine/mirror/Mirror.h>
#include <lib/engine/mirror/MirrorUtils.h>
#include "lib/engine/render/renderable/BaseRenderableFactory.h"
#include "lib/engine/utils/Utils.h"

using LayoutSelector = std::function<WeightedLayouts(RenderableType type)>;

template<typename C>
using RenderablesAndMirrors = std::pair<WeightedRenderables<C>, MirrorSelector<C> >;

template<typename C>
using RenderableSelector = std::function<RenderablesAndMirrors<C>(uint16_t layoutId)>;

using RenderableParamSelector = std::function<Params(
    RenderableType type,
    TypeInfo::ID renderableId,
    uint16_t layoutId,
    Mirror mirror
)>;

template<typename C>
using RandomRenderable = std::tuple<uint16_t, RenderableFactoryRef<C>, Mirror>;

class LayoutConfig {
    const std::map<uint16_t, String> _layoutNames;
    const RenderableSelector<CRGB> _effects;
    const RenderableSelector<CRGB> _overlays;
    const RenderableSelector<uint8_t> _transitions;
    const LayoutSelector _layoutSelector;
    const RenderableParamSelector _paramSelector;

    template<typename T>
    RandomRenderable<T> randomEntry(
        RenderableType type,
        const RenderableSelector<T> &renderableSelector,
        RenderableFactoryRef<T> defaultRenderableFactory
    ) const;

public:
    const std::set<uint16_t> layoutIds;

    explicit LayoutConfig(
        const std::set<uint16_t> &layoutIds,
        const std::map<uint16_t, String> &layoutNames,
        LayoutSelector layoutSelector,
        RenderableSelector<CRGB> effects,
        RenderableSelector<CRGB> overlays,
        RenderableSelector<uint8_t> transitions,
        RenderableParamSelector paramSelector
    ): _layoutNames(layoutNames),
       _layoutSelector(std::move(layoutSelector)),
       _effects(std::move(effects)),
       _overlays(std::move(overlays)),
       _transitions(std::move(transitions)),
       _paramSelector(std::move(paramSelector)),
       layoutIds(layoutIds) {
    }

    String layoutName(uint16_t layoutId) const {
        if (_layoutNames.find(layoutId) == _layoutNames.end()) {
            return "UNKNOWN(" + String(layoutId) + ")";
        }
        return _layoutNames.at(layoutId);
    }

    RandomRenderable<CRGB> randomEffect() const;

    RandomRenderable<CRGB> randomOverlay() const;

    RandomRenderable<uint8_t> randomTransition() const;

    Params params(
        RenderableType type,
        TypeInfo::ID renderableId,
        uint16_t layoutId,
        Mirror mirror
    ) const;

    virtual
    ~LayoutConfig() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutId(
    const std::vector<uint16_t> &layoutIds,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCONFIG_H
