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

const uint8_t GradientEffect::PARAM_START;
const uint8_t GradientEffect::PARAM_VARIATION;

static const GradientEffectFactory factoryInstance;
EffectFactoryRef<CRGB> GradientEffect::factory = &factoryInstance;

void GradientEffect::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    float percent = static_cast<float>(variation) / (255.0f * 2.0f);
    uint8_t increment = max(1, 255.0f / static_cast<float>(effectArraySize));
    uint8_t adjustedIncrement = random8(increment * (1.0f - percent), increment * (1.0f + percent));

    uint8_t progressOffset = start + progress * 255;

    for (int i = 0; i < effectArraySize; i++) {
        effectArray[i] = ColorFromPalette(
            context.palette.palette,
            progressOffset + (i * max(1, adjustedIncrement))
        );
    }
}
