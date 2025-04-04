#include "utils.h"
#include <cstdio>
#include <string>
#include "FastLED.h"

String CRGBToHex(const CRGB &color) {
    char hexColor[7];
    sprintf(hexColor, "%02X%02X%02X", color.r, color.g, color.b);
    return String(hexColor);
}

uint16_t unsignedModulo(int value, uint16_t modulo) {
    return ((value % modulo) + modulo) % modulo;
}

uint8_t increaseContrast(uint8_t noise) {
    return map(noise, 50, 190, 0, 255);
}