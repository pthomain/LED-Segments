#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <string>
#include <memory>
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

#endif //UTILS_H