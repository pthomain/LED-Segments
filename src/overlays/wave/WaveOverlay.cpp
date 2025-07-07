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

#include "WaveOverlay.h"
#include "engine/effect/Effect.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

static const WaveOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> WaveOverlay::factory = &factoryInstance;

void WaveOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    memset(effectArray, CRGB::White, effectArraySize * sizeof(CRGB));

    uint8_t ceiling = effectArraySize;
    uint8_t floor = effectArraySize / 1;

    uint8_t amplitude = beatsin16(
        bpm,
        floor,
        ceiling
    );

    phase += scrollingSpeed;

    uint32_t index = isClockwise ? context.nbSegments - segmentIndex : segmentIndex;
    uint16_t x = (index * nbWaves * 65536L / context.nbSegments) + phase;

    //Sine value in range -32768 to +32767
    int16_t sineVal = map(sin16(x), -32768, 32767, 0, amplitude);

    memset(effectArray, CRGB::Black, sineVal * sizeof(CRGB));
}
