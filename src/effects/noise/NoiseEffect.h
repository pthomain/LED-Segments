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

#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include "engine/effect/Effect.h"

class NoiseEffect : public Effect<CRGB>, public Effect<CRGB>::Factory<NoiseEffect> {
protected:
    const uint8_t noiseScale = random8(10, 20);
    const uint8_t paletteScale = random8(2, 6);
    const uint8_t noiseSpeed = random8(5, 100);

public:
    explicit NoiseEffect(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    String name() const override { return "Noise"; }
    EffectType type() const override { return EffectType::EFFECT; }

    static EffectFactory<CRGB> factory;
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
