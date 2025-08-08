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

#include "GradientEffect.h"
#include "engine/utils/Weights.h"

const uint8_t GradientEffect::PARAM_COLOUR_START;
const uint8_t GradientEffect::PARAM_DENSITY_VARIATION;

static const GradientEffectFactory factoryInstance;
EffectFactoryRef<CRGB> GradientEffect::factory = &factoryInstance;

void GradientEffect::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    uint8_t progressOffset = colourStart + static_cast<uint8_t>(progress * 255);
    auto increment = max(1.0f, 255.0f / static_cast<float>(effectArraySize)) + variation8;

    for (int pixelIndex = 0; pixelIndex < effectArraySize; pixelIndex++) {
        effectArray[pixelIndex] = ColorFromPalette(
            context.palette.palette,
            progressOffset + (pixelIndex * increment)
        );
    }
}
