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

#include "StackEffect.h"

EffectFactory<CRGB> StackEffect::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<StackEffect>(effectContext);
};

void StackEffect::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    //TODO
    auto increment = static_cast<uint8_t>(variation * (255.0f / static_cast<float>(effectArraySize)));

    if (bottomColour == 0 || topColour == 0) {
        bottomColour = ColorFromPalette(context.palette.palette, currentColourIndex);
        topColour = ColorFromPalette(context.palette.palette, currentColourIndex + increment);
    } else if (timeInMillis % speed == 0) {
        currentColourIndex += increment;
        bottomColour = topColour;
        topColour = ColorFromPalette(context.palette.palette, (currentColourIndex + increment) % 255);
    }

    auto stackProgress = (timeInMillis % speed) / static_cast<float>(speed);
    auto stackOffset = effectArraySize - (stackProgress * effectArraySize);

    for (int i = 0; i < effectArraySize; i++) {
        effectArray[i] = i < stackOffset ? bottomColour : topColour;
    }
}
