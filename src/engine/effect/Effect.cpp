//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>
//
// /*
//  * This file is part of LED Segments.
//  *
//  * LED Segments is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * LED Segments is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with LED Segments. If not, see <https://www.gnu.org/licenses/>.
//  *
//

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

#include "Effect.h"

//TODO must be offset by the cumulative number of LEDs for the pixel at pos cycleStep
void Effect::fillArray(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t frameIndex
) {
    uint8_t delta = max(1, 255 / effectArraySize); //TODO use percent
    if (frameIndex % cycleSpeed == 0) {
        linearCycleStep = unsignedModulo(linearCycleStep + delta, 255);
        if (circularCycleStep == 0 || circularCycleStep >= 255) isCycleReversed = !isCycleReversed;
        circularCycleStep += isCycleReversed ? delta : -delta;
    }
    fillArrayInternal(effectArray, effectArraySize, frameIndex);
};