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

#include "utils/Utils.h"
#include "engine/display/Display.h"
#include "specs/phrase/PhraseSpec.h"
#include "specs/fibonacci/FibonacciSpec.h"

//TODO add stack highlight, each pixel stacks on the previous one + reverse
//TODO add swipe effect, one colour slides over the previous one
//TODO add word highlight modifier, increase luminosity on each word for a while
//TODO add chase with trail modifier, like ping pong but with a trail
//TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
//TODO add a Composite effect that picks a different effect for each segment or the same effect but a different palette

#define LED_PIN 9
#define BRIGHTNESS 10
#define MIN_EFFECT_DURATION_IN_SECONDS 5
#define MAX_EFFECT_DURATION_IN_SECONDS 5

//TODO add effect burst

Display *display;

void setup() {
    if constexpr (IS_DEBUG) {
        Serial.begin(9600);
        delay(1000);
    }

    auto *displaySpec = new PhraseSpec();

    display = Display::create<LED_PIN, GRB>(
        *displaySpec,
        IS_DEBUG ? 10 : 128,
        MIN_EFFECT_DURATION_IN_SECONDS,
        MAX_EFFECT_DURATION_IN_SECONDS,
        2000
    );
}

void loop() {
    display->loop();
}
