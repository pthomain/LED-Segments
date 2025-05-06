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

#ifndef LED_SEGMENTS_TESTPHRASECONFIG_H
#define LED_SEGMENTS_TESTPHRASECONFIG_H

#define NB_LEDS 256
#define NB_LETTERS 10
#define NB_WORDS 3

constexpr static uint16_t LETTERS[NB_LETTERS][2] = {
    {0, 23},
    {24, 39},
    {40, 63},
    {64, 103},
    {104, 151},
    {152, 207},
    {208, 231},
    {232, 239},
    {240, 247},
    {248, 255}
};

constexpr static uint16_t WORDS[NB_WORDS][2] = {
    {0, 103},
    {104, 231},
    {232, 255}
};

#endif //LED_SEGMENTS_TESTPHRASECONFIG_H
