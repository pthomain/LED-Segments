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

#include "SparkleOverlay.h"
#include "crgb.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

const uint16_t SparkleOverlay::PARAM_DENSITY;

static const SparkleOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> SparkleOverlay::factory = &factoryInstance;

void SparkleOverlay::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    fract16 progress,
    unsigned long timeElapsedInMillis
) {
    for (uint16_t i = 0; i < segmentSize; i++) {
        segmentArray[i] = probability(density) ? CRGB::White : CRGB::Black;
    }
}

} // namespace LEDSegments
