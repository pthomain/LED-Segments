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

#include "UmbrellaSpec.h"

uint16_t UmbrellaSpec::nbSegments(uint16_t layoutId) const {
    switch (layoutId) {
        case LEDS_IN_SPOKE: return NB_SPOKES;
        case SPOKES_IN_WHOLE: return 1; //WHOLE segment
        default: {
            Serial.print("UmbrellaSpec::nbSegments: Unknown layoutId ");
            Serial.println(layoutId);
            return 0; //This should not happen
        }
    }
}

uint16_t UmbrellaSpec::segmentSize(uint16_t layoutId, uint16_t segmentIndex) const {
    switch (layoutId) {
        case LEDS_IN_SPOKE: return LEDS_PER_SPOKE;
        case SPOKES_IN_WHOLE: return NB_SPOKES;
        default: {
            Serial.print("UmbrellaSpec::nbPixels: Unknown layoutId ");
            Serial.println(layoutId);
            return 0; //This should not happen
        }
    }
}

void UmbrellaSpec::mapLeds(
    uint16_t layoutId,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    float progress,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    auto mapSingleLed = [&](uint16_t ledIndex, const std::function<void(uint16_t)> &onLedMapped) {
        onLedMapped(ledIndex);
    };

    switch (layoutId) {
        case LEDS_IN_SPOKE: mapSingleLed(segmentIndex * LEDS_PER_SPOKE + pixelIndex, onLedMapped);
            break;

        case SPOKES_IN_WHOLE:
            for (uint8_t ledIndex = 0; ledIndex < LEDS_PER_SPOKE; ledIndex++) {
                mapSingleLed(pixelIndex * LEDS_PER_SPOKE + ledIndex, onLedMapped); // PIXEL == SPOKE
            }
            break;

        default: break;
    }
}
