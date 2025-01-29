#ifndef LED_SEGMENTS_FIBONACCIDISPLAY_H
#define LED_SEGMENTS_FIBONACCIDISPLAY_H

#include <cstdint>
#include "engine/displayspec.h"
#include "FibonacciEnums.h"

class FibonacciSpec : public DisplaySpec {

private:
    void applyColourToPixel(
            const uint16_t variation,
            const uint16_t segmentIndex,
            uint16_t pixelIndex,
            int8_t inflexionPoint,
            CRGB *outputArray,
            const CRGB colour
    ) const;

    void applyColourToPixelUnit(
            const uint16_t variation,
            const uint16_t pixelUnitIndex,
            int8_t inflexionPoint,
            CRGB *outputArray,
            const CRGB colour
    ) const;

public :

    explicit FibonacciSpec() : DisplaySpec() {}

    uint16_t nbLeds() const override { return TOTAL_FIBONACCI_LEDS; }

    uint16_t nbLayouts() const override { return VARIATIONS; }

    String layoutName(const uint16_t layoutIndex) const override;

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

    void setColour(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            const uint16_t pixelIndex,
            const uint16_t frameIndex,
            CRGB *outputArray,
            const CRGB colour
    ) const override;

    ~FibonacciSpec() override = default;
};

#endif //LED_SEGMENTS_FIBONACCIDISPLAY_H
