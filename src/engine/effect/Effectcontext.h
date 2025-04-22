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


#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "utils/Utils.h"
#include "Transition.h"

class EffectContext {
public:
    const boolean isDisplayCircular;
    const uint16_t layoutIndex;
    const uint16_t effectIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const Transition transition;
    const uint8_t transitionLayoutIndex;
    const Mirror transitionMirror;

    EffectContext(
        const boolean isDisplayCircular,
        const uint16_t layoutIndex,
        const uint16_t effectIndex,
        const CRGBPalette16 &palette,
        const Mirror mirror,
        const Transition transition,
        const uint8_t transitionLayoutIndex,
        const Mirror transitionMirror
    ) : isDisplayCircular(isDisplayCircular),
        layoutIndex(layoutIndex),
        effectIndex(effectIndex),
        palette(palette),
        mirror(mirror),
        transition(transition),
        transitionLayoutIndex(transitionLayoutIndex),
        transitionMirror(transitionMirror) {
    }
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
