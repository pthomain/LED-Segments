#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>
#include <vector>
#include "FastLED.h"

constexpr bool IS_DEBUG = true;

void printNumber(char text[], int value);

String CRGBToHex(const CRGB &color);

uint16_t unsignedModulo(int index, uint16_t modulo);

uint8_t increaseContrast(uint8_t noise);

static const std::vector<CRGBPalette16> PALETTES PROGMEM = std::vector<CRGBPalette16>{
    PartyColors_p,
    PartyColors_p,
    PartyColors_p,
    ForestColors_p,
    CloudColors_p,
    OceanColors_p,
};

static const std::vector<uint8_t> PRIMES = std::vector<uint8_t>{
    43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
    101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
    151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251
};

int freeMemory();

#endif //UTILS_H
