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

#ifndef PALETTE_H
#define PALETTE_H

#include <vector>
#include "FastLED.h"

class Palette {
public:
    const CRGBPalette16 palette;
    const String name;

    Palette(
        const CRGBPalette16 &palette,
        const String &name
    ) : palette(palette), name(name) {
    }
};

static const Palette RAINBOW_PALETTE = Palette(Rainbow_gp, "Rainbow");
static const Palette NO_PALETTE = Palette(CRGBPalette16(), "None");

static const auto PALETTES = std::vector{
    Palette(CRGBPalette16(
                0xff0000,
                0x00ff00
            ),
            "Red-Green"
    ),
    Palette(CRGBPalette16(
                0x00ff00,
                0x0000ff
            ),
            "Green-Blue"
    ),
    Palette(CRGBPalette16(
                0x0000ff,
                0xff0000
            ),
            "Blue-Red"
    ),
};


#endif //PALETTE_H
