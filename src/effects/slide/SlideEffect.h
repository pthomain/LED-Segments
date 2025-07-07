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

#ifndef LED_SEGMENTS_SLIDEEFFECT_H
#define LED_SEGMENTS_SLIDEEFFECT_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"

class SlideEffect : public Effect<SlideEffect, CRGB> {
    const uint8_t nbColours = 4;
    const uint8_t step = 255 / nbColours;
    const uint8_t start = random8();

    uint8_t *colourIndexForSegment;
    uint16_t *headPositionForSegment;

public:
    explicit SlideEffect(const EffectContext &effectContext)
        : Effect(effectContext),
          colourIndexForSegment(new uint8_t[nbColours]),
          headPositionForSegment(new uint16_t[context.nbSegments]) {
        memset(colourIndexForSegment, 0, nbColours * sizeof(uint8_t));
        memset(headPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    ~SlideEffect() override {
        delete[] colourIndexForSegment;
        delete[] headPositionForSegment;
    }

    static constexpr const char *name() { return "SlideEffect"; }
    static constexpr WeightedOperations operations() { return just(EffectOperation::EFFECT); }
    static EffectFactoryRef<CRGB> factory;
};

class SlideEffectFactory : public EffectFactory<SlideEffect, CRGB> {};

#endif //LED_SEGMENTS_SLIDEEFFECT_H
