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

#include "SlideEffect.h"
#include "colorutils.h"

EffectFactory<CRGB> SlideEffect::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<SlideEffect>(effectContext);
};

void SlideEffect::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    const auto bottomColourIndex = bottomColourIndexForSegment[segmentIndex];
    const auto topColourIndex = (bottomColourIndex + 1) % nbColours;
    const auto headPosition = headPositionForSegment[segmentIndex];

    auto topColourStart = CHSV(start + topColourIndex * (255 / nbColours), 255, 255);
    auto topColourEnd = CHSV(start + 127 + topColourIndex * (255 / nbColours), 255, 255);

    auto bottomColourStart = CHSV(start + bottomColourIndex * (255 / nbColours), 255, 255);
    auto bottomColourEnd = CHSV(start + 127 + bottomColourIndex * (255 / nbColours), 255, 255);

    fill_gradient(effectArray, 0, bottomColourStart, effectArraySize - 1, bottomColourEnd, TGradientDirectionCode::SHORTEST_HUES);
    fill_gradient(effectArray, 0, topColourStart, headPosition, topColourEnd, TGradientDirectionCode::SHORTEST_HUES);

    if (headPosition == effectArraySize - 1) {
        bottomColourIndexForSegment[segmentIndex] = topColourIndex;
        headPositionForSegment[segmentIndex] = 0;
    } else {
        headPositionForSegment[segmentIndex] = (timeInMillis / 50) % effectArraySize;
    }
}
