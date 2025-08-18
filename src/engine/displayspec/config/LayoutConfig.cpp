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

#include "LayoutConfig.h"
#include "engine/utils/Utils.h"
#include "transitions/none/NoTransition.h"
#include <effects/none/NoEffect.h>
#include <overlays/none/NoOverlay.h>

namespace LEDSegments {

template<typename T>
RandomRenderable<T> LayoutConfig::randomEntry(
    RenderableType type,
    const RenderableSelector<T> &renderableSelector,
    RenderableFactoryRef<T> defaultRenderableFactory
) const {
    uint16_t randomLayoutId = pickRandomWeightedItem(_layoutSelector(type), uint16_t{0});

    auto [renderables, renderableMirrorSelector] = renderableSelector(randomLayoutId);

    if (!renderables.empty()) {
        auto renderableFactory = pickRandomWeightedItem(renderables, defaultRenderableFactory);
        auto randomMirror = pickRandomWeightedItem(renderableMirrorSelector(renderableFactory), Mirror::NONE);
        return RandomRenderable<T>{randomLayoutId, renderableFactory, randomMirror};
    }

    return RandomRenderable<T>{randomLayoutId, defaultRenderableFactory, Mirror::NONE};
}

RandomRenderable<CRGB> LayoutConfig::randomEffect() const {
    return randomEntry(RenderableType::EFFECT, _effects, NoEffect::factory);
}

RandomRenderable<uint8_t> LayoutConfig::randomTransition() const {
    return randomEntry(RenderableType::TRANSITION, _transitions, NoTransition::factory);
}

RandomRenderable<CRGB> LayoutConfig::randomOverlay() const {
    return randomEntry(RenderableType::OVERLAY, _overlays, NoOverlay::factory);
}

Params LayoutConfig::params(
    RenderableType type,
    TypeInfo::ID renderableId,
    uint16_t layoutId,
    Mirror mirror
) const {
    return _paramSelector(type, renderableId, layoutId, mirror);
}

} // namespace LEDSegments
