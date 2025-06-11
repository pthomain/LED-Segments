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

static const Palette NO_PALETTE = Palette(CRGBPalette16(), "None");

static const auto PALETTES = std::vector{
    Palette(
        CRGBPalette16{
            0xff0066,
            0x9900ff,
            0x3333ff
        },
        "Aurora"
    ),
    Palette(
        CRGBPalette16{
            0xff7003,
            0x17f702,
            0xf3f702,
            0xf70202
        },
        "Jamaica"
    ),
    Palette(
        CRGBPalette16{
            0xff0000,
            0xffffff,
            0x0000ff,
            0xffffff,
        },
        "France"
    )
};

#endif //PALETTE_H
