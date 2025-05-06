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

#ifndef LED_SEGMENTS_DLH_PHRASECONFIG_H
#define LED_SEGMENTS_DLH_PHRASECONFIG_H

#define NB_LEDS 191
#define NB_LETTERS 14
#define NB_WORDS 5

#include <cstdint>

constexpr static const uint16_t LETTERS[NB_LETTERS][2] = {
    {0, 12},
    {13, 25},
    {26, 41},
    {42, 56},
    {57, 74},

    {75, 82},

    {83, 93},
    {94, 104},
    {105, 117},
    {118, 135},

    {136, 143},

    {144, 161},
    {162, 177},
    {178, 190}
};

constexpr static const uint16_t WORDS[NB_WORDS][2] = {
    {0, 74},
    {75, 82},
    {83, 135},
    {136, 143},
    {144, 190}
};

#endif //LED_SEGMENTS_DLH_PHRASECONFIG_H
