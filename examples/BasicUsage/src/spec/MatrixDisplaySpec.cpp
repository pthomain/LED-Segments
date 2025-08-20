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
        case LED_1_ROW_1:
        case LED_2_ROW_1:
        case LED_4_ROW_1:
        case LED_8_ROW_1:
            return 32;

        case LED_1_ROW_2:
        case LED_2_ROW_2:
        case LED_4_ROW_2:
        case LED_8_ROW_2:
            return 16;

        case LED_1_ROW_4:
        case LED_2_ROW_4:
        case LED_4_ROW_4:
        case LED_8_ROW_4:
            return 8;

        default: {
#ifdef DEBUG
            Serial.print("MatrixDisplaySpec::nbSegments: Unknown layoutId ");
            Serial.println(layoutId);
#endif
            return 1; //Fallback to prevent division by zero
        }
    }
}

// Provides the number of virtual pixels for each given layoutId and segmentIndex.
// Make sure to return the number of virtual pixels here as defined in LayoutConfig.h and NOT the number of LEDs.
// The mapping to actual LEDs is only ever done in the mapLeds method, everywhere else virtual pixels are used.
uint16_t MatrixDisplaySpec::segmentSize(uint16_t layoutId, uint16_t segmentIndex) const {
    switch (layoutId) {
        case LED_1_ROW_1:
        case LED_1_ROW_2:
        case LED_1_ROW_4:
            return 8;

        case LED_2_ROW_1:
        case LED_2_ROW_2:
        case LED_2_ROW_4:
            return 4;

        case LED_4_ROW_1:
        case LED_4_ROW_2:
        case LED_4_ROW_4:
            return 2;

        case LED_8_ROW_1:
        case LED_8_ROW_2:
        case LED_8_ROW_4:
            return 1;

        default: {
#ifdef DEBUG
            Serial.print("MatrixDisplaySpec::nbPixels: Unknown layoutId ");
            Serial.println(layoutId);
#endif
            return 1; //Fallback to prevent division by zero
        }
    }
}

// This method maps the given layout, segment and pixel index to the corresponding LED index on the matrix display.
void MatrixDisplaySpec::mapLeds(
    uint16_t layoutId, //represents the layoutId as defined in LayoutConfig.h
    uint16_t segmentIndex, //represents the segment index within the given layoutId
    uint16_t pixelIndex, //represents the relative pixel index within the given segment
    fract16 progress,
    //represents the effect's progress, used for dynamic displays whose mapping might evolve over the duration of the effect
    const std::function<void(uint16_t)> &onLedMapped
    //callback function to be called for each mapped LED in the given pixel
) const {
    uint8_t rowsPerSegment;
    switch (layoutId) {
        case LED_1_ROW_1:
        case LED_2_ROW_1:
        case LED_4_ROW_1:
        case LED_8_ROW_1:
            rowsPerSegment = 1;
            break;
        case LED_1_ROW_2:
        case LED_2_ROW_2:
        case LED_4_ROW_2:
        case LED_8_ROW_2:
            rowsPerSegment = 2;
            break;
        case LED_1_ROW_4:
        case LED_2_ROW_4:
        case LED_4_ROW_4:
        case LED_8_ROW_4:
            rowsPerSegment = 4;
            break;
        default:
            rowsPerSegment = 1;
            break;
    }
    
    uint16_t segmentLengthInLeds = rowsPerSegment * 8; // 8 LEDs per row
    uint16_t segmentOffset = segmentIndex * segmentLengthInLeds;

    uint16_t pixelLengthInLeds = 8 / segmentSize(layoutId, segmentIndex);
    uint16_t pixelOffset = pixelIndex * pixelLengthInLeds;

    for (uint16_t row = 0; row < rowsPerSegment; ++row) {
        for (uint16_t pixel = 0; pixel < pixelLengthInLeds; ++pixel) {
            onLedMapped(segmentOffset + row * 8 + pixelOffset + pixel);
        }
    }
}
