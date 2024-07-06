#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>
#include <memory>
#include <vector>
#include "FastLED.h"

void printNumber(char text[], int value);

// Implementation of std::make_unique for compilers that don't support C++14
namespace std {
    template<class T, class... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

typedef enum {
    SCOPE_LETTER = 0,
    SCOPE_WORD = 1,
    SCOPE_WHOLE = 2
} Scope;

typedef enum {
    UNIT_PIXEL,
    UNIT_LETTER,
    UNIT_WORD
} PixelUnit;

typedef enum {
    MIRROR_NONE,
    MIRROR_CENTRE,
    MIRROR_EDGE
} Mirror;

std::string scopeToString(Scope scope);

std::string pixelUnitToString(PixelUnit pixelUnit);

std::string mirrorToString(Mirror mirror);

std::string CRGBToHex(const CRGB &color);

static const std::vector<CRGBPalette16> PALETTES = std::vector<CRGBPalette16>{
        PartyColors_p,
        PartyColors_p,
        PartyColors_p,
        ForestColors_p,
        CloudColors_p,
        OceanColors_p,
//        LavaColors_p
};

static const std::vector<uint8_t> PRIMES = std::vector<uint8_t>{
        0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
        43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
        101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
        151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
        199, 211, 223, 227, 229, 233, 239, 241, 251
};

#endif //UTILS_H