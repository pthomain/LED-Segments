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

#ifndef LED_SEGMENTS_WAVEOVERLAY_H
#define LED_SEGMENTS_WAVEOVERLAY_H

#include "engine/effect/Effect.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

class WaveOverlay : public Effect<WaveOverlay, CRGB> {

    const bool isClockwise = probability(0.5f);
    const uint16_t scrollingSpeed = random8(2, 4) * 64;
    const uint8_t bpm = random(1, 5) * 5;
    const uint8_t nbWaves = random8(1, 8);
    uint16_t phase = 0;

public:
    explicit WaveOverlay(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "WaveOverlay"; }

    static constexpr WeightedOperations operations() {
        return {
            {EffectOperation::OVERLAY_MULTIPLY, 4},
            {EffectOperation::OVERLAY_INVERT, 1}
        };
    }

    static EffectFactoryRef<CRGB> factory;
};

class WaveOverlayFactory : public EffectFactory<WaveOverlay, CRGB> {
};

#endif //LED_SEGMENTS_WAVEOVERLAY_H
