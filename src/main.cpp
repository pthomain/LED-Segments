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

#include "engine/display/Display.h"
#include "engine/utils/Utils.h"
#include "specs/phrase/PhraseSpec.h"
#include "specs/fibonacci/FibonacciSpec.h"
#include "specs/hat/HatSpec.h"
#include "specs/umbrella/UmbrellaSpec.h"

using SPEC = FibonacciSpec;
Display<SPEC> *display;

void setup() {
    if constexpr (IS_DEBUG) {
        Serial.begin(9600);
        delay(2000);
    }
    display = new Display<SPEC>(); //must be instantiated in this method
}

void loop() {
    display->loop();
}
