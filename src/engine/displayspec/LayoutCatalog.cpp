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

#include <string>
#include <engine/effect/none/NoEffect.h>
#include <engine/overlay/none/NoOverlay.h>
#include "engine/transitions/none/NoTransition.h"
#include "engine/utils/Utils.h"

template<typename T>
RandomEffect<T> LayoutCatalog::randomEntry(
    EffectType effectType,
    const EffectSelector<T> &effectSelector,
    EffectFactoryRef<T> defaultEffectFactory
) const {
    uint16_t randomLayoutId = pickRandomWeightedItem(_layoutSelector(effectType), uint16_t{0});

    auto [effects, effectMirrorSelector] = effectSelector(randomLayoutId);

    auto effectFactory = pickRandomWeightedItem(effects, defaultEffectFactory);
    auto randomMirror = pickRandomWeightedItem(effectMirrorSelector(effectFactory), Mirror::NONE);

    return RandomEffect<T>{randomLayoutId, effectFactory, randomMirror};
}

RandomEffect<CRGB> LayoutCatalog::randomEffect() const {
    return randomEntry(EffectType::EFFECT, _effects, NoEffect::factory);
}

RandomEffect<uint8_t> LayoutCatalog::randomTransition() const {
    return randomEntry(EffectType::TRANSITION, _transitions, NoTransition::factory);
}

RandomEffect<CRGB> LayoutCatalog::randomOverlay() const {
    return randomEntry(EffectType::OVERLAY, _overlays, NoOverlay::factory);
}
