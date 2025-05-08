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

enum class PaletteType {
    GRADIENT, // used for effects using the given palette for complex gradients
    DISCRETE, // used for effects using individual colours from the given palette
    NONE // used for effects that don't use a palette
};

class Palette {
    const CRGBPalette16 _palette;
    const PaletteType _type;

public:
    Palette(
        const CRGBPalette16 &palette,
        const PaletteType type
    ) : _palette(palette), _type(type) {
    }

    const CRGBPalette16 &palette() const {
        return _palette;
    }

    const PaletteType type() const {
        return _type;
    }
};

static const Palette NO_PALETTE = Palette(CRGBPalette16(), PaletteType::NONE);

#endif //PALETTE_H
