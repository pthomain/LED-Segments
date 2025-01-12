#ifndef LED_SEGMENTS_FIBONACCIDISPLAY_H
#define LED_SEGMENTS_FIBONACCIDISPLAY_H

#include <cstdint>
#include "displayspec/displaySpec.h"

enum FibonacciLayout {
    PIXEL_IN_SPIRAL,
    SEGMENT_IN_SPIRAL,

    PIXEL_IN_SPIRAL_INFLEXION_20,
    PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC,
    PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN,
    PIXEL_IN_SPIRAL_SYMMETRIC,

    SEGMENT_IN_SPIRAL_INFLEXION_20,
    SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC,
    SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN,
    SEGMENT_IN_SPIRAL_SYMMETRIC,

    SEGMENT_IN_CONCENTRIC,
    PIXEL_IN_CONCENTRIC,
};

class FibonacciSpec : public DisplaySpec {

private:
    void applyColourToPixel(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            uint16_t pixelIndex,
            int8_t inflexionPoint,
            CRGB *outputArray,
            const CRGB colour
    ) const;

    void applyColourToSpiralSegment(
            const uint16_t layoutIndex,
            const uint16_t segmentIndex,
            int8_t inflexionPoint,
            CRGB *outputArray,
            const CRGB colour
    ) const;

    void applyColourToSpiralPixel(
            const uint16_t layoutIndex,
            const uint16_t pixelIndex,
            int8_t inflexionPoint,
            CRGB *outputArray,
            const CRGB colour
    ) const;

public :

    explicit FibonacciSpec() : DisplaySpec() {}

    uint16_t nbLeds() const override { return 324; }

    uint16_t nbLayouts() const override { return 12; }

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
