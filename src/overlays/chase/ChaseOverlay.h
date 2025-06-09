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

#ifndef CHASEOVERLAY_H
#define CHASEOVERLAY_H

#include "engine/effect/Effect.h"

class ChaseOverlay : public Effect<CRGB>, public Effect<CRGB>::Factory<ChaseOverlay> {
    const float sparkDensity = .1f;
    const uint8_t sparkLength = 1;
    const uint8_t speed = 5; //pixels per second
    unsigned long *lastSparkTimeMillisForSegment;
    bool *forward;
    bool *backward;
    bool *tempForward;
    bool *tempBackward;

public:
    explicit ChaseOverlay(const EffectContext &effectContext): Effect(effectContext),
                                                               lastSparkTimeMillisForSegment(
                                                                   new unsigned long[context.nbSegments]),
                                                               forward(new bool[context.maxSegmentSize]),
                                                               backward(new bool[context.maxSegmentSize]),
                                                               tempForward(new bool[context.maxSegmentSize]),
                                                               tempBackward(new bool[context.maxSegmentSize]) {
        memset(lastSparkTimeMillisForSegment, 0, sizeof(unsigned long) * context.nbSegments);
        memset(forward, 0, sizeof(bool) * context.maxSegmentSize);
        memset(backward, 0, sizeof(bool) * context.maxSegmentSize);
        memset(tempForward, 0, sizeof(bool) * context.maxSegmentSize);
        memset(tempBackward, 0, sizeof(bool) * context.maxSegmentSize);
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    String name() const override { return "Chase"; }
    EffectType type() const override { return EffectType::OVERLAY_ALPHA; }

    ~ChaseOverlay() override {
        delete[] forward;
        delete[] backward;
    }

    static EffectFactory<CRGB> factory;
};

#endif //CHASEOVERLAY_H
