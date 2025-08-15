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

#include "Blending.h"

uint8_t multiply(uint8_t base, uint8_t overlay) {
    return static_cast<uint16_t>(base * overlay) / 255;
}

CRGB multiply(const CRGB &base, const CRGB &overlay) {
    return {
        multiply(base.r, overlay.r),
        multiply(base.g, overlay.g),
        multiply(base.b, overlay.b)
    };
}

uint8_t screen(uint8_t base, uint8_t overlay) {
    return 255 - static_cast<uint16_t>((255 - base) * (255 - overlay)) / 255;
}

CRGB screen(const CRGB &base, const CRGB &overlay) {
    return {
        screen(base.r, overlay.r),
        screen(base.g, overlay.g),
        screen(base.b, overlay.b)
    };
}

uint8_t invert(uint8_t base, uint8_t overlay) {
    return overlay == CRGB::Black ? base : base - 128;
}

CRGB invert(const CRGB &base, const CRGB &overlay) {
    if (overlay == CRGB::Black) {
        return base;
    }

    CHSV hsv = rgb2hsv_approximate(base);
    hsv.h += 128;
    return hsv;
}
