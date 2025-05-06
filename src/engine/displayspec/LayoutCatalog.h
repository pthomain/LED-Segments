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
#include <vector>
#include <engine/effect/Effect.h>
#include <engine/mirror/Mirror.h>

static const String UNKNOWN = "UNKNOWN";
static const String EFFECT_ENTRY = "effect";
static const String OVERLAY_ENTRY = "overlay";
static const String MIRROR_ENTRY = "mirror";
static const String TRANSITION_ENTRY = "transition";

class LayoutCatalog {
    const uint16_t _nbLayouts;
    const std::map<uint16_t, String> _layoutNames;
    const std::map<uint16_t, std::vector<EffectFactory> > _effects;
    const std::map<uint16_t, std::vector<EffectFactory> > _overlays;
    const std::map<uint16_t, std::vector<EffectFactory> > _transitions;
    const std::map<uint16_t, std::vector<Mirror> > _mirrors;
    const float probabilityOfOverlay;

    template<typename T>
    T randomMapEntryForLayout(
        const String &entryType,
        uint16_t layoutIndex,
        const std::map<uint16_t, std::vector<T> > &map,
        const T &defaultValue
    ) const;

    template<typename T>
    std::pair<uint16_t, T> randomLayoutSpecificEntry(
        const String &entryType,
        const std::map<uint16_t, std::vector<T> > &map,
        const std::pair<uint16_t, T> &defaultValue
    ) const;

public:
    explicit LayoutCatalog(
        const uint16_t nbLayouts,
        std::map<uint16_t, String> layoutNames,
        std::map<uint16_t, std::vector<EffectFactory> > effects,
        std::map<uint16_t, std::vector<EffectFactory> > overlays,
        std::map<uint16_t, std::vector<EffectFactory> > transitions,
        std::map<uint16_t, std::vector<Mirror> > mirrors,
        const float probabilityOfOverlay = 0.0f
    ) : _nbLayouts(nbLayouts),
        _layoutNames(std::move(layoutNames)),
        _effects(std::move(effects)),
        _overlays(std::move(overlays)),
        _transitions(std::move(transitions)),
        _mirrors(std::move(mirrors)),
        probabilityOfOverlay(probabilityOfOverlay) {
    }

    uint16_t nbLayouts() const {
        return _nbLayouts;
    }

    virtual String layoutName(uint16_t layoutIndex) const {
        return _layoutNames.find(layoutIndex) == _layoutNames.end() ? UNKNOWN : _layoutNames.at(layoutIndex);
    }

    EffectFactory randomEffectFactory(uint16_t layoutIndex) const;

    Mirror randomMirror(uint16_t layoutIndex) const;

    std::pair<uint16_t, EffectFactory> randomTransition() const;

    std::pair<uint16_t, EffectFactory> randomOverlay() const;

    virtual ~LayoutCatalog() = default;
};

template<typename T>
std::map<uint16_t, std::vector<T> > mapLayoutIndex(
    const std::vector<uint16_t> &layoutIndexes,
    const std::function<std::vector<T>(uint16_t)> &mapper
);

#endif //LAYOUTCATALOG_H
