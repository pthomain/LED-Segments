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

#ifndef NOTRANSITION_H
#define NOTRANSITION_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

class NoTransition : public Transition<NoTransition> {
public:
    explicit NoTransition(const RenderableContext &context) : Transition(context) {
    }

    void fillSegmentArray(
        uint8_t *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "NoTransition"; }
    static RenderableFactoryRef<uint8_t> factory;
};

class NoTransitionFactory : public RenderableFactory<NoTransitionFactory, NoTransition, uint8_t> {
public:
    static Params declareParams() {
        return {};
    }
};

} // namespace LEDSegments

#endif //NOTRANSITION_H