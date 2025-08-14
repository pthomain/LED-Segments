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

#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <Arduino.h>
#include <vector>
#include "FastLED.h"
#include <functional>

#define IS_DEBUG true

static const std::vector<uint8_t> PRIMES PROGMEM = std::vector<uint8_t>{
    43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
    101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
    151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251
};

static String CRGBToHex(const CRGB &color) {
    char hexColor[7];
    sprintf(hexColor, "%02X%02X%02X", color.r, color.g, color.b);
    return String(hexColor);
}

static uint16_t unsignedModulo(int value, uint16_t modulo) {
    return ((value % modulo) + modulo) % modulo;
}

//Ensures noise values are between 0 and 255 (they usually are between 50 and 190 by default)
static uint8_t normaliseNoise(uint8_t noise) {
    return map(constrain(noise, 50, 190), 50, 190, 0, 255);
}

inline bool probability(const float probability) {
    if (probability == 0.0f) return false;
    if (probability >= 1.0f) return true;
    return random16() < static_cast<uint32_t>(probability * 65535.0f);
}

static void mapLedInSnakeDisplay(
    const uint16_t ledIndex,
    const std::function<void(uint16_t)> &onLedMapped,
    const uint8_t ledsPerRow = 8
) {
    const uint16_t rowIndex = ledIndex / ledsPerRow;
    auto rowStart = rowIndex * ledsPerRow;
    auto rowEnd = rowStart + ledsPerRow - 1;
    auto relativeIndex = ledIndex - rowStart;
    onLedMapped(rowEnd - relativeIndex);
}

inline void fillEffectPalette(
    CRGB *effectArray,
    uint16_t effectArraySize,
    const CRGBPalette16 &effectPalette
) {
    fill_palette(
        effectArray,
        effectArraySize,
        0,
        max(1, 255 / effectArraySize),
        effectPalette,
        255,
        LINEARBLEND
    );
}

template<typename T>
static bool contains(const std::vector<T> &vector, const T &value) {
    return std::find(vector.begin(), vector.end(), value) != vector.end();
}

extern "C" char *sbrk(int incr);

inline int freeMemoryXiao() {
    char stack_dummy = 0;
    char *heap_end = sbrk(0);
    return &stack_dummy - heap_end;
}

#endif //UTILS_H
