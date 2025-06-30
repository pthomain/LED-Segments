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
#include "engine/utils/Utils.h"

class ChaseOverlay : public Effect<ChaseOverlay, CRGB> {
    const uint8_t minxSparksPerSegment = 1;
    const uint8_t maxSparksPerSegment = 5;
    const uint8_t sparksPerSegment = random8(minxSparksPerSegment, maxSparksPerSegment);

    const uint8_t intervalBetweenSparks = 10;
    const uint8_t trailLength = 3;

    uint16_t leadingSparkPosition = 0;
    uint16_t *sparkIntervalCounterPerSegment;
    uint8_t *nbSparksForSegment;

    // Whether the sparks bounce back and forth. Otherwise, new sparks get emitted at the start after they exit.
    bool isBouncy = probability(0.75f);
    bool isSwirling = probability(0.5f);

    std::vector<std::vector<bool> > forward;
    std::vector<std::vector<bool> > backward;

    bool *tempForward;
    bool *tempBackward;

public:
    explicit ChaseOverlay(const EffectContext &effectContext)
        : Effect(effectContext),
          sparkIntervalCounterPerSegment(new uint16_t[context.nbSegments]),
          nbSparksForSegment(new uint8_t[context.nbSegments]),
          forward(std::vector<std::vector<bool> >(context.nbSegments)),
          backward(std::vector<std::vector<bool> >(context.nbSegments)),
          tempForward(new bool[context.maxSegmentSize]),
          tempBackward(new bool[context.maxSegmentSize]) {
        memset(tempForward, 0, sizeof(bool) * context.maxSegmentSize);
        memset(tempBackward, 0, sizeof(bool) * context.maxSegmentSize);
        memset(sparkIntervalCounterPerSegment, 0, sizeof(uint16_t) * context.nbSegments);
        memset(nbSparksForSegment, 0, sizeof(uint8_t) * context.nbSegments);

        for (int i = 0; i < context.nbSegments; i++) {
            for (int j = 0; j < context.maxSegmentSize; j++) {
                forward[i].push_back(false);
                backward[i].push_back(false);
            }
        }
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "ChaseOverlay"; }
    static constexpr EffectType type() { return EffectType::OVERLAY_MULTIPLY; }

    ~ChaseOverlay() {
        delete[] sparkIntervalCounterPerSegment;
        delete[] nbSparksForSegment;
        delete[] tempForward;
        delete[] tempBackward;
    }

    static const EffectFactory<CRGB>& factory;
};

class ChaseOverlayFactory : public EffectFactory<CRGB> {
public:
    std::unique_ptr<BaseEffect<CRGB> > create(const EffectContext &context) const override {
        return std::make_unique<ChaseOverlay>(context);
    }

    const char *name() const override {
        return ChaseOverlay::name();
    }
};

#endif //CHASEOVERLAY_H
