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

#include "HatSpec.h"
#include <vector>
#include <map>

const std::vector<uint8_t> &HatSpec::eyeCircle(uint16_t pixelIndex) {
    static const std::vector<uint8_t> *eyeCircles = new std::vector<uint8_t>[4]{
        {18},
        {17, 11, 12, 19, 25, 24},
        {16, 10, 5, 6, 7, 13, 20, 26, 31, 30, 29, 23},
        {15, 9, 4, 0, 1, 2, 3, 8, 14, 21, 27, 32, 36, 35, 34, 33, 28, 22}
    };
    static const std::vector<uint8_t> emptyVector = {};

    if (pixelIndex < 4) {
        return eyeCircles[pixelIndex];
    }

    return emptyVector;
}

uint16_t HatSpec::nbSegments(uint16_t layoutId) const {
    return 2;
}

uint16_t HatSpec::segmentSize(uint16_t layoutId, uint16_t segmentIndex) const {
    if (segmentIndex == 0) {
        return layoutId == CONCENTRIC_EYES ? EYE_CIRCLES : LEDS_PER_EYE;
    }
    return LEDS_PER_PANEL;
}

void HatSpec::mapLeds(
    uint16_t layoutId,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    float progress,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    if (segmentIndex == 0) {
        uint8_t prefix = NB_PANELS * LEDS_PER_PANEL;
        if (layoutId == CONCENTRIC_EYES) {
            for (const auto &ledIndex: eyeCircle(pixelIndex)) {
                onLedMapped(prefix + ledIndex);
                onLedMapped(prefix + ledIndex + LEDS_PER_EYE);
            }
        } else {
            onLedMapped(prefix + pixelIndex);
            onLedMapped(prefix + pixelIndex + LEDS_PER_EYE);
        }
    } else {
        for (uint8_t i = 0; i < NB_PANELS; i++) {
            onLedMapped(i * LEDS_PER_PANEL + pixelIndex);
        }
    }
}
