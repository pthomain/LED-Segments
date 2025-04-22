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

#pragma once

#ifndef LED_SEGMENTS_SEEDGENERATOR_H
#define LED_SEGMENTS_SEEDGENERATOR_H

#include <cstdint>
#include "Arduino.h"

//Provide any free pin to use to gather electrical noise to build entropy for the PRNG.
//Be careful not to include any pin already used in your circuit.
void addEntropy(const uint8_t *freePins, const uint8_t nbFreePins) {
    uint8_t entropy = 0;
    for (uint8_t i = 0; i < nbFreePins; i++) {
        entropy = (entropy << 1) | (analogRead(i) & 1); //Use only LSB for more entropy
    }
    random16_add_entropy(entropy);
}

#endif //LED_SEGMENTS_SEEDGENERATOR_H
