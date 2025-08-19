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

#include "MatrixDisplaySpec.h"

// Provides the number of segments for each given layoutId as defined in LayoutConfig.h.
uint16_t MatrixDisplaySpec::nbSegments(uint16_t layoutId) const {
    switch (layoutId) {
        // Make sure to use the number of virtual pixels here and not the number of LEDs, see segmentSize() method.
        case GROUP_BY_1: return TOTAL_PIXELS;
        case GROUP_BY_2: return TOTAL_PIXELS / 2;
        case GROUP_BY_4: return TOTAL_PIXELS / 4;
        case GROUP_BY_8: return TOTAL_PIXELS / 8;
        case GROUP_BY_16: return TOTAL_PIXELS / 16;
        default: {
            Serial.print("MatrixDisplaySpec::nbSegments: Unknown layoutId ");
            Serial.println(layoutId);
            return 0; //This should not happen
        }
    }
}

// Provides the number of virtual pixels for each given layoutId and segmentIndex.
// Make sure to return the number of virtual pixels here as defined in LayoutConfig.h and NOT the number of LEDs.
// The mapping to actual LEDs is only ever done in the mapLeds method, everywhere else virtual pixels are used.
uint16_t MatrixDisplaySpec::segmentSize(uint16_t layoutId, uint16_t segmentIndex) const {
    switch (layoutId) {
        case GROUP_BY_1: return 1;     // 1 virtual pixel per segment (8 LEDs per pixel, as defined in LayoutConfig.h)
        case GROUP_BY_2: return 2;     // 2 virtual pixels per segment (2x8 LEDs per pixel)
        case GROUP_BY_4: return 4;     // 4 virtual pixels per segment (4x8 LEDs per pixel)
        case GROUP_BY_8: return 8;     // 8 virtual pixels per segment (8x8 LEDs per pixel)
        case GROUP_BY_16: return 16;   // 16 virtual pixels per segment (16x8 LEDs per pixel)
        default: {
            Serial.print("MatrixDisplaySpec::nbPixels: Unknown layoutId ");
            Serial.println(layoutId);
            return 0; //This should not happen
        }
    }
}

// This method maps the given layout, segment and pixel index to the corresponding LED index on the matrix display.
void MatrixDisplaySpec::mapLeds(
    uint16_t layoutId,     //represents the layoutId as defined in LayoutConfig.h
    uint16_t segmentIndex, //represents the segment index within the given layoutId
    uint16_t pixelIndex,   //represents the relative pixel index within the given segment
    float progress, //represents the effect's progress, used for dynamic displays whose mapping might evolve over the duration of the effect
    const std::function<void(uint16_t)> &onLedMapped //callback function to be called for each mapped LED in the given pixel
) const {
    //convert virtual pixels count to actual LED count
    uint16_t ledsPerSegment = segmentSize(layoutId, segmentIndex) * PIXEL_SIZE;
    uint16_t segmentStart = segmentIndex * ledsPerSegment;

    for(uint16_t i = 0; i < ledsPerSegment; ++i) {
        onLedMapped(segmentStart + i); //call the callback for each LED index in the current segment (upper bound is checked internally)
    }
}
