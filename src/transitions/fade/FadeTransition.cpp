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

#include "FadeTransition.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

static const FadeTransitionFactory factoryInstance;
RenderableFactoryRef<uint8_t> FadeTransition::factory = &factoryInstance;

void FadeTransition::fillSegmentArray(
    uint8_t *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    fract16 progress,
    unsigned long timeElapsedInMillis
) {
    auto alpha = static_cast<uint8_t>(255.0f * progress);
    memset(segmentArray, alpha, segmentSize * sizeof(uint8_t));
}

} // namespace LEDSegments
