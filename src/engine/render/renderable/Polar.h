#ifndef LED_SEGMENTS_SPECS_POLAR_H
#define LED_SEGMENTS_SPECS_POLAR_H

#include "FastLED.h"
#include <functional>

using PolarCoords = std::pair<uint16_t, fract16>;
using PolarCoordsMapper = std::function<PolarCoords(uint16_t pixelIndex)>;

#endif //LED_SEGMENTS_SPECS_POLAR_H
