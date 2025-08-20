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
    LED_1_ROW_1,
    LED_2_ROW_1,
    LED_4_ROW_1,
    LED_8_ROW_1,

    LED_1_ROW_2,
    LED_2_ROW_2,
    LED_4_ROW_2,
    LED_8_ROW_2,

    LED_1_ROW_4,
    LED_2_ROW_4,
    LED_4_ROW_4,
    LED_8_ROW_4,
};

// Set of layout IDs being used
static const std::set<uint16_t> layoutIds = {
    LED_1_ROW_1,
    LED_2_ROW_1,
    LED_4_ROW_1,
    LED_8_ROW_1,

    LED_1_ROW_2,
    LED_2_ROW_2,
    LED_4_ROW_2,
    LED_8_ROW_2,

    LED_1_ROW_4,
    LED_2_ROW_4,
    LED_4_ROW_4,
    LED_8_ROW_4,
};

// Match layout IDs to their names (for console debugging purposes)
static const std::map<uint16_t, String> layoutNames = {
    {LED_1_ROW_1, "Pixel = 1 LED; Segment = 1 row = 8 LEDs"}, //using virtual pixels here, not LEDs
    {LED_2_ROW_1, "Pixel = 2 LEDs; Segment = 1 row = 8 LEDs"},
    {LED_4_ROW_1, "Pixel = 4 LEDs; Segment = 1 row = 8 LEDs"},
    {LED_8_ROW_1, "Pixel = 8 LEDs; Segment = 1 row = 8 LEDs"},

    {LED_1_ROW_2, "Pixel = 1 LED; Segment = 2 rows = 16 LEDs"},
    {LED_2_ROW_2, "Pixel = 2 LEDs; Segment = 2 rows = 16 LEDs"},
    {LED_4_ROW_2, "Pixel = 4 LEDs; Segment = 2 rows = 16 LEDs"},
    {LED_8_ROW_2, "Pixel = 8 LEDs; Segment = 2 rows = 16 LEDs"},

    {LED_1_ROW_4, "Pixel = 1 LED; Segment = 4 rows = 32 LEDs"},
    {LED_2_ROW_4, "Pixel = 2 LEDs; Segment = 4 rows = 32 LEDs"},
    {LED_4_ROW_4, "Pixel = 4 LEDs; Segment = 4 rows = 32 LEDs"},
    {LED_8_ROW_4, "Pixel = 8 LEDs; Segment = 4 rows = 32 LEDs"},
};

static WeightedLayouts layoutSelector(RenderableType type) {
    switch (type) {
        case RenderableType::EFFECT: //layouts being used for effects
            return {
                //weights influence the probability of each layout being randomly selected
                {LED_1_ROW_1, 1},
                {LED_2_ROW_1, 1},
                {LED_4_ROW_1, 1},
                {LED_8_ROW_1, 1},
                {LED_1_ROW_2, 1},
                {LED_2_ROW_2, 1},
                {LED_4_ROW_2, 1},
                {LED_8_ROW_2, 1},
                {LED_1_ROW_4, 1},
                {LED_2_ROW_4, 1},
                {LED_4_ROW_4, 1},
                {LED_8_ROW_4, 1},
            };

        case RenderableType::OVERLAY: //layouts being used for overlays
        case RenderableType::TRANSITION: //layouts being used for transitions
        default:
            return {
                {LED_1_ROW_4, 1},
                {LED_8_ROW_1, 1},
            };
    }
}

#endif // TEST_LAYOUT_CONFIG_H
