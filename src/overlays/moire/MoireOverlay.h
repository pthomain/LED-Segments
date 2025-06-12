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

#ifndef MOIREOVERLAY_H
#define MOIREOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/utils/Utils.h"

class MoireOverlay : public Effect<CRGB>, public Effect<CRGB>::Factory<MoireOverlay> {

    const bool isInverted = probability(0.5f);
    const bool isReversed = probability(0.5f);
    const bool isDoubleSpiral = false;//probability(0.5f);

    const CRGB frontColour = isInverted ? CRGB::White : CRGB::Black;
    const CRGB backColour = isInverted ? CRGB::Black : CRGB::White;

    const uint8_t headLength = 5;

    uint16_t *headPositionForSegment;
    CRGB *reverseArray;

public:
    explicit MoireOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          reverseArray(new CRGB[context.maxSegmentSize]),
          headPositionForSegment(new uint16_t[context.nbSegments]) {
        memset(headPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    String name() const override { return "Moiré"; }
    EffectType type() const override { return EffectType::OVERLAY_MULTIPLY; }

    static EffectFactory<CRGB> factory;

    MoireOverlay(const MoireOverlay &) = delete;

    ~MoireOverlay() override {
        delete[] headPositionForSegment;
    }
};

#endif //MOIREOVERLAY_H
