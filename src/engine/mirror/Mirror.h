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

#pragma once

#ifndef LED_SEGMENTS_MIRROR_H
#define LED_SEGMENTS_MIRROR_H

#include "FastLED.h"

enum class Mirror {
    NONE,
    REVERSE,
    CENTRE,
    EDGE,
    REPEAT,
    REPEAT_REVERSE
};

uint16_t getMirrorSize(Mirror mirror, uint16_t effectArraySize);

template<typename C>
void applyMirror(Mirror mirror, C *effectArray, uint16_t effectArraySize);

String getMirrorName(Mirror mirror);

#endif //LED_SEGMENTS_MIRROR_H
