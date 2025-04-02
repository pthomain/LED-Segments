#ifndef LED_SEGMENTS_SIMPLEPIXELMAPPER_H
#define LED_SEGMENTS_SIMPLEPIXELMAPPER_H

#include "engine/render/pixelmapper.h"
#include "engine/displayspec/displayspec.h"
#include "WString.h"

class SimplePixelMapper : PixelMapper {

private:
    const DisplaySpec &displaySpec;

public:

    explicit SimplePixelMapper(const DisplaySpec &displaySpec) : displaySpec(displaySpec) {}

    void mapPixels(
            const String &rendererName,
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t segmentSize,
            const uint16_t frameIndex,
            CRGB *outputArray,
            CRGB *effectArray
    ) override;
};

#endif //LED_SEGMENTS_SIMPLEPIXELMAPPER_H
