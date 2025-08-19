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

#include "NoiseEffect.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

const uint8_t NoiseEffect::PARAM_NOISE_SCALE;
const uint8_t NoiseEffect::PARAM_PALETTE_SCALE;
const uint8_t NoiseEffect::PARAM_NOISE_SPEED;

static const NoiseEffectFactory factoryInstance;
RenderableFactoryRef<CRGB> NoiseEffect::factory = &factoryInstance;

void NoiseEffect::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    const uint8_t increment = max(1, timeElapsedInMillis / speedDivider);
    const uint8_t noise = inoise8(noiseScale, randomStart + increment);
    const uint8_t step = max(1, 255 / (segmentSize * paletteScale));

    fill_palette(
        segmentArray,
        segmentSize,
        normaliseNoise(noise),
        step,
        context.palette.palette,
        255,
        LINEARBLEND
    );
}

} // namespace LEDSegments
