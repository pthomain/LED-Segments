//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

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

#include "NoiseEffect.h"
#include <functional>

EffectFactory NoiseEffect::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<NoiseEffect>(effectContext);
};

void NoiseEffect::fillArrayInternal(
    CRGB *effectArray,
    const uint16_t effectArraySize,
    const uint16_t frameIndex
) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        const uint8_t noiseScale = beatsin8(10, 10, 30);
        const uint8_t noise = inoise8(i * noiseScale, millis() / noiseSpeed);

        //        bool usePalette = effectContext.iteration % 2 == 0;
        bool usePalette = false;
        if (usePalette) {
            fill_palette(
                effectArray,
                effectArraySize,
                increaseContrast(noise),
                10,
                effectContext.palette,
                255,
                LINEARBLEND
            );
        } else {
            effectArray[i] = CHSV(increaseContrast(noise), 255, 255); //noise rainbow
        }
    }
};

//TODO OctaveEffect: https://www.youtube.com/watch?v=7Dhh0IMSI4Q&ab_channel=ScottMarley
