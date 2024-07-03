#include "utils.h"
#include <cstdio>
#include <string>
#include "FastLED.h"

void printNumber(char text[], int value) {
    auto string = std::string(text);
    string.append(": ");
    string.append(std::to_string(value));
    Serial.println(string.data());
}

std::string scopeToString(Scope scope) {
    switch (scope) {
        case SCOPE_LETTER:
            return "SCOPE_LETTER";
        case SCOPE_WORD:
            return "SCOPE_WORD";
        case SCOPE_WHOLE:
            return "SCOPE_WHOLE";
        default:
            return "UNKNOWN_SCOPE";
    }
}

std::string pixelUnitToString(PixelUnit pixelUnit) {
    switch (pixelUnit) {
        case UNIT_PIXEL:
            return "UNIT_PIXEL";
        case UNIT_LETTER:
            return "UNIT_LETTER";
        case UNIT_WORD:
            return "UNIT_WORD";
        default:
            return "UNKNOWN_UNIT";
    }
}

std::string mirrorToString(Mirror mirror) {
    switch (mirror) {
        case MIRROR_NONE:
            return "MIRROR_NONE";
        case MIRROR_CENTRE:
            return "MIRROR_CENTRE";
        case MIRROR_EDGE:
            return "MIRROR_EDGE";
        default:
            return "UNKNOWN_MIRROR";
    }
}

std::string CRGBToHex(const CRGB& color) {
    char hexColor[7];
    sprintf(hexColor, "%02X%02X%02X", color.r, color.g, color.b);
    return std::string(hexColor);
}