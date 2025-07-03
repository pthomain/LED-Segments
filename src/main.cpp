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
#include "specs/umbrella/UmbrellaSpec.h"

//TODO add stack effect, each pixel stacks on the previous one + reverse
//TODO add swipe effect, one colour slides over the previous one (divide palette by ~6)

// #define LED_PIN D7 //Umbrella
#define LED_PIN 9
#define BRIGHTNESS 255
#define DEBUG_BRIGHTNESS 50
#define MIN_EFFECT_DURATION_IN_SECONDS 5
#define MAX_EFFECT_DURATION_IN_SECONDS 10
#define DEBUG_EFFECT_DURATION_IN_SECONDS 30

Display *display;

void setup() {
    if constexpr (IS_DEBUG) {
        Serial.begin(9600);
        delay(2000);
    }

    display = Display::create<LED_PIN, GRB>(
        std::make_unique<UmbrellaSpec>(),
        IS_DEBUG ? DEBUG_BRIGHTNESS : BRIGHTNESS,
        IS_DEBUG ? DEBUG_EFFECT_DURATION_IN_SECONDS : MIN_EFFECT_DURATION_IN_SECONDS,
        IS_DEBUG ? DEBUG_EFFECT_DURATION_IN_SECONDS : MAX_EFFECT_DURATION_IN_SECONDS,
        IS_UMBRELLA ? 500 : 2000,
        30
    );
}

void loop() {
    display->loop();
}
