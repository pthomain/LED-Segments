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

#ifndef DASHOVERLAY_H
#define DASHOVERLAY_H

#include "engine/effect/Effect.h"

class DashOverlay : public Effect<CRGB>, public Effect<CRGB>::Factory<DashOverlay> {
    uint16_t *headPositionForSegment;
    uint16_t *tailPositionForSegment;
    bool *isReversedForSegment;

    const uint8_t tailSpeed = random8(1, 3);

public:
    explicit DashOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          headPositionForSegment(new uint16_t[context.nbSegments]),
          tailPositionForSegment(new uint16_t[context.nbSegments]),
          isReversedForSegment(new bool[context.nbSegments]) {
        memset(headPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
        memset(tailPositionForSegment, 0, context.nbSegments * sizeof(uint16_t));
        memset(isReversedForSegment, 0, context.nbSegments * sizeof(bool));
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    String name() const override { return "Dash"; }
    EffectType type() const override { return EffectType::OVERLAY_MULTIPLY; }

    static EffectFactory<CRGB> factory;

    ~DashOverlay() override {
        delete[] headPositionForSegment;
        delete[] tailPositionForSegment;
        delete[] isReversedForSegment;
    }
};

#endif //DASHOVERLAY_H
