#include "utils.h"
#include <cstdio>
#include <string>
#include <vector>
#include "FastLED.h"

void printNumber(char text[], int value) {
    auto string = std::string(text);
    string.append(": ");
    string.append(std::to_string(value));
    Serial.println(string.data());
}

std::string CRGBToHex(const CRGB& color) {
    char hexColor[7];
    sprintf(hexColor, "%02X%02X%02X", color.r, color.g, color.b);
    return std::string(hexColor);
}

extern "C" char* sbrk(int i);
int freeMemory() {
    char stack_dummy = 0;
    return &stack_dummy - sbrk(0);
}