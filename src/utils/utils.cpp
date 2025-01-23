#include "utils/utils.h"
#include <cstdio>
#include <string>
#include "FastLED.h"

std::string CRGBToHex(const CRGB& color) {
    char hexColor[7];
    sprintf(hexColor, "%02X%02X%02X", color.r, color.g, color.b);
    return std::string(hexColor);
}
