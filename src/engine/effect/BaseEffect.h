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

#ifndef BASEEFFECT_H
#define BASEEFFECT_H

#include <utility>
#include "EffectContext.h"
#include "engine/utils/Weights.h"

template<typename C>
class BaseEffect {
protected:
    unsigned long start = 0L;
    bool isFirstFrame = true;
    uint8_t randomStart;
    uint16_t frameIndex = 0;

    virtual void fillArrayInternal(
        C *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeInMillis
    ) = 0;

    EffectOperation _effectOperation = EffectOperation::EFFECT;
    bool _effectOperationInitialised = false;

public:
    const EffectContext context;

    void fillArray(
        C *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress
    );

    explicit BaseEffect(EffectContext context)
        : context(std::move(context)),
          randomStart(random8()) {
    }

    virtual const char *effectName() = 0;

    virtual WeightedOperations effectOperations() = 0;

    EffectOperation effectOperation() {
        if (!_effectOperationInitialised) {
            _effectOperation = pickRandomWeightedItem(
                effectOperations(),
                EffectOperation::EFFECT
            );
            _effectOperationInitialised = true;
        }
        return _effectOperation;
    }

    virtual ~BaseEffect() = default;
};

template class BaseEffect<CRGB>;
template class BaseEffect<uint8_t>;

#endif //BASEEFFECT_H
