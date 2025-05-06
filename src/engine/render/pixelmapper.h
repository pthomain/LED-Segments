//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

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

#ifndef LED_SEGMENTS_PIXELMAPPER_H
#define LED_SEGMENTS_PIXELMAPPER_H

#include "WString.h"
#include "crgb.h"

class PixelMapper {
public:
    PixelMapper() = default;

    virtual void mapPixels(
        const String &rendererName,
        uint16_t layoutIndex,
        uint16_t segmentIndex,
        uint16_t segmentSize,
        float progress,
        CRGB *outputArray,
        CRGB *segmentArray
    ) = 0;

    virtual ~PixelMapper() = default;
};

#endif //LED_SEGMENTS_PIXELMAPPER_H
