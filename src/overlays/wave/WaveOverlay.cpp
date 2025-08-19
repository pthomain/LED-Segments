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

#include "WaveOverlay.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

const uint16_t WaveOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint16_t WaveOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const WaveOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> WaveOverlay::factory = &factoryInstance;

void WaveOverlay::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    memset(segmentArray, CRGB::White, segmentSize * sizeof(CRGB));

    uint8_t amplitude = beatsin16(
        bpm,
        segmentSize,
        segmentSize
    );

    phase += scrollingSpeed;

    uint32_t index = isClockwise ? context.nbSegments - segmentIndex : segmentIndex;
    uint16_t x = (index * nbWaves * 65536L / context.nbSegments) + phase;

    //Sine value in range -32768 to +32767
    int16_t sineVal = map(sin16(x), -32768, 32767, 0, amplitude);

    memset(segmentArray, CRGB::Black, sineVal * sizeof(CRGB));
}

} // namespace LEDSegments
