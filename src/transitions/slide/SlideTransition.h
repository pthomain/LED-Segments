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

#ifndef SLIDETRANSITION_H
#define SLIDETRANSITION_H

#include "engine/render/renderable/TypedRenderable.h"
#include "engine/render/renderable/BaseRenderableFactory.h"

class SlideTransition : public Transition<SlideTransition> {
public:
    explicit SlideTransition(const RenderableContext &context) : Transition(context) {
    }

    void fillArrayInternal(
        uint8_t *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "SlideTransition"; }
    static RenderableFactoryRef<uint8_t> factory;
};

class SlideTransitionFactory : public RenderableFactory<SlideTransitionFactory, SlideTransition, uint8_t> {
public:
    static Params declareParams() {
        return {};
    }
};

#endif //SLIDETRANSITION_H