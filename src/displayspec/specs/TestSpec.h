#ifndef LED_SEGMENTS_TESTSPEC_H
#define LED_SEGMENTS_TESTSPEC_H

#include "displayspec/displayspec.h"
#include "FastLED.h"

class TestSpec : public DisplaySpec {

private :

    void setColourForSimpleRows(
            const uint16_t segmentIndex,
            const uint16_t pixelIndex,
            CRGB *outputArray,
            CRGB colour
    ) const;

public :

    explicit TestSpec() : DisplaySpec(256, 1) {}

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

    void setColour(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t pixelIndex,
            CRGB *outputArray,
            CRGB colour
    ) const override;

    ~TestSpec() override = default;
};

#endif //LED_SEGMENTS_TESTSPEC_H
