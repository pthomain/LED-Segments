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

#ifndef TEST_LAYOUT_CONFIG_H
#define TEST_LAYOUT_CONFIG_H

#include "engine/displayspec/config/LayoutConfig.h"

constexpr uint16_t PIXEL_SIZE = 8; //LEDS are grouped by 8 to represent a single virtual pixel
constexpr uint16_t TOTAL_PIXELS = 32; //Total number of pixels on the matrix display
constexpr uint16_t TOTAL_LEDS = TOTAL_PIXELS * PIXEL_SIZE; //Total number of LEDs on the matrix display (256)

// Define an enum entry for each layout
enum Layout {
    GROUP_BY_1,
    GROUP_BY_2,
    GROUP_BY_4,
    GROUP_BY_8,
    GROUP_BY_16
};

// Set of layout IDs being used
static const std::set<uint16_t> layoutIds = {
    GROUP_BY_1,
    GROUP_BY_2,
    GROUP_BY_4,
    GROUP_BY_8,
    GROUP_BY_16
};

// Match layout IDs to their names (for console debugging purposes)
static const std::map<uint16_t, String> layoutNames = {
    { GROUP_BY_1, "Group pixels by 1" }, //using virtual pixels here, not LEDs
    { GROUP_BY_2, "Group pixels by 2" },
    { GROUP_BY_4, "Group pixels by 4" },
    { GROUP_BY_8, "Group pixels by 8" },
    { GROUP_BY_16, "Group pixels by 16" }
};

static WeightedLayouts layoutSelector(RenderableType type) {
    switch (type) {
        case RenderableType::EFFECT: //layouts being used for effects
            return {
                {GROUP_BY_1, 8}, //weights influence the probability of each layout being randomly selected
                {GROUP_BY_2, 6},
                {GROUP_BY_4, 4},
                {GROUP_BY_8, 2},
                {GROUP_BY_16, 1}
            };

        case RenderableType::OVERLAY: //layouts being used for overlays
            return just(uint16_t(GROUP_BY_2));

        case RenderableType::TRANSITION: //layouts being used for transitions
        default:
            return just(uint16_t(GROUP_BY_1));
    }
}

#endif // TEST_LAYOUT_CONFIG_H
