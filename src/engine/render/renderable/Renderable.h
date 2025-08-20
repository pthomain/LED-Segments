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

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "RenderableContext.h"
#include "engine/utils/Weights.h"
#include <utility>

namespace LEDSegments {

template<typename Data>
class Renderable {
    unsigned long effectStartInMillis = 0L;
    uint16_t _frameIndex = 0;

protected:
    uint16_t frameIndex() const {
        return _frameIndex;
    }

    virtual void fillSegmentArray(
        Data *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) = 0;

    RenderableOperation _renderableOperation = RenderableOperation::EFFECT;
    bool _renderableOperationInitialised = false;

public:
    const RenderableContext context;

    virtual void fillArray(
        Data *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress
    ) final;

    virtual void beforeFrame(
        fract16 progress,
        unsigned long timeElapsedInMillis
    ) = 0;

    virtual void afterFrame(
        fract16 progress,
        unsigned long timeElapsedInMillis
    ) = 0;

    explicit Renderable(RenderableContext context): context(std::move(context)) {
    }

    virtual WeightedOperations renderableOperations() = 0;

    RenderableOperation renderableOperation() {
        if (!_renderableOperationInitialised) {
            _renderableOperation = pickRandomWeightedItem(
                renderableOperations(),
                RenderableOperation::EFFECT
            );
            _renderableOperationInitialised = true;
        }
        return _renderableOperation;
    }

    virtual ~Renderable() = default;
};

template class Renderable<CRGB>;
template class Renderable<uint8_t>;

} // namespace LEDSegments

#endif // RENDERABLE_H
