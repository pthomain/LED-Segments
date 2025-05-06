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

#include "SeedGenerator.h"
#include <Arduino.h>
#include "lib8tion/random8.h"
#include <vector>

void addEntropy(
    const std::vector<uint8_t> &freePinsForEntropy
) {
    static bool seedSet = false;
    uint8_t entropy = 0;

    if (!seedSet) {
        for (auto pin: freePinsForEntropy) {
            pinMode(pin, INPUT);
        }
    }

    for (uint8_t i = 0; i < 16; i++) {
        entropy = (entropy << 1) | (analogRead(freePinsForEntropy[i % freePinsForEntropy.size()]) & 1);
    }

    if (seedSet) {
        random16_add_entropy(entropy);
    } else {
        random16_set_seed(entropy);
        seedSet = true;
    }
}
