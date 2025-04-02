#ifndef LED_SEGMENTS_PIXELMAPPER_H
#define LED_SEGMENTS_PIXELMAPPER_H

#include "WString.h"
#include "FastLED.h"

class PixelMapper {

public:

    PixelMapper() = default;

    virtual void mapPixels(
            const String &rendererName,
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t segmentSize,
            const uint16_t frameIndex,
            CRGB *outputArray,
            CRGB *effectArray
    ) = 0;

};

#endif //LED_SEGMENTS_PIXELMAPPER_H
