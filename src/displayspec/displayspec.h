#ifndef LED_SEGMENTS_DISPLAYSPEC_H
#define LED_SEGMENTS_DISPLAYSPEC_H

#include <cstdint>
#include "FastLED.h"

class DisplaySpec {

public:
    const uint16_t totalLeds;
    const uint16_t nbLayouts;

    explicit DisplaySpec(
            const uint16_t totalLeds,
            const uint16_t nbLayouts
    ) : totalLeds(totalLeds),
        nbLayouts(nbLayouts) {}

    virtual uint16_t nbSegments(const uint16_t layoutIndex) const = 0;

    virtual uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const = 0;

    virtual void setColour(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t pixelIndex,
            CRGB *outputArray,
            CRGB colour
    ) const = 0;

    virtual ~DisplaySpec() = default;
};

#endif //LED_SEGMENTS_DISPLAYSPEC_H
