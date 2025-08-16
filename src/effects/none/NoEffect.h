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

#ifndef NOEFFECT_H
#define NOEFFECT_H

#include "engine/render/renderable/TypedRenderable.h"
#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/utils/Weights.h"

class NoEffect : public Effect<NoEffect> {
public:
    explicit NoEffect(const RenderableContext &context) : Effect(context) {
    }

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "NoEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class NoEffectFactory : public RenderableFactory<NoEffectFactory, NoEffect, CRGB> {
public:
    static Params declareParams() {
        return {};
    }
};

#endif //NOEFFECT_H
