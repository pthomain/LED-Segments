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

#include "CustomEffect.h"
#include "crgb.h"
#include "engine/utils/Weights.h"

// Required fields to register the effect
static const CustomEffectFactory factoryInstance;
RenderableFactoryRef<CRGB> CustomEffect::factory = &factoryInstance;

// Method called by the Renderer for each segment in the selected layout.
// Effects should only fill the segmentArray with the desired colour.
void CustomEffect::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeInMillis
) {
    CRGB colour;
    switch (segmentIndex % 3) {
        case 0: colour = CRGB::Red; break;
        case 1: colour = CRGB::Green; break;
        case 2:
        default: colour = CRGB::Blue; break;
    }

    fill_solid(segmentArray, segmentSize, colour);
}
