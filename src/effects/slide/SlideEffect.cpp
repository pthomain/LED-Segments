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
#include "engine/utils/Weights.h"

namespace LEDSegments {

const uint8_t SlideEffect::PARAM_NB_COLOURS;
const uint8_t SlideEffect::PARAM_COLOUR_START;

static const SlideEffectFactory factoryInstance;
RenderableFactoryRef<CRGB> SlideEffect::factory = &factoryInstance;

void SlideEffect::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    const auto headPosition = headPositionForSegment.get()[segmentIndex];

    const uint8_t bottomColourIndex = segmentIndex % nbColours;
    const uint8_t topColourIndex = (bottomColourIndex + 1) % nbColours;

    const auto bottomColour = colourIndexForSegment.get()[bottomColourIndex];
    const auto topColour = colourIndexForSegment.get()[topColourIndex];

    auto topColourStart = CHSV(colourStart + topColour * (255 / nbColours), 255, 255);
    auto topColourEnd = CHSV(colourStart + 127 + topColour * (255 / nbColours), 255, 255);

    auto bottomColourStart = CHSV(colourStart + bottomColour * (255 / nbColours), 255, 255);
    auto bottomColourEnd = CHSV(colourStart + 127 + bottomColour * (255 / nbColours), 255, 255);

    fill_gradient(
        segmentArray,
        0,
        bottomColourStart,
        segmentSize - 1,
        bottomColourEnd,
        TGradientDirectionCode::SHORTEST_HUES
    );

    fill_gradient(
        segmentArray,
        0,
        topColourStart,
        headPosition,
        topColourEnd,
        TGradientDirectionCode::SHORTEST_HUES
    );

    if (headPosition == segmentSize - 1) {
        colourIndexForSegment.get()[bottomColourIndex] = topColour;
        headPositionForSegment.get()[segmentIndex] = 0;
    } else {
        headPositionForSegment.get()[segmentIndex] = (timeInMillis / speedDivider) % segmentSize;
    }
}

} // namespace LEDSegments
