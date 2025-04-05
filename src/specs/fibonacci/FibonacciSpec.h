#ifndef LED_SEGMENTS_FIBONACCIDISPLAY_H
#define LED_SEGMENTS_FIBONACCIDISPLAY_H

#include <algorithm>
#include <cstdint>
#include "FibonacciEnums.h"
#include "crgb.h"
#include "engine/displayspec/displayspec.h"

class FibonacciSpec : public DisplaySpec {
    const std::vector<uint8_t> variations = computeVariations();
    std::vector<uint16_t> transitionLayouts = std::vector<uint16_t>();
    std::vector<uint16_t> allLayouts = std::vector<uint16_t>();

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


    std::pair<uint16_t, uint8_t> radialToSpiralIndex(
        const uint8_t segmentIndex,
        const uint8_t radialIndex
    ) const;

    uint8_t getLedOffset(const uint8_t pixelIndex) const;

    uint8_t getLedPadding(const uint8_t pixelIndex) const;

public :
    explicit FibonacciSpec() {
        std::copy(variations.begin(), variations.end(), transitionLayouts.begin());
        std::copy(variations.begin(), variations.end(), allLayouts.begin());
    }

    uint16_t nbLeds() const override { return TOTAL_FIBONACCI_LEDS; }

    uint16_t nbLayouts() const override { return variations.size(); }

    String layoutName(const uint16_t layoutIndex) const override;

    bool isCircular() const override { return true; }

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t nbPixels(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

    void setColour(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        const uint16_t frameIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const override;

    std::vector<EffectFactory> effects() const override { return NO_EFFECTS; }
    std::vector<EffectFactory> highlights() const override { return NO_EFFECTS; }
    std::vector<uint16_t> matchLayouts(LayoutDescription description) const override;

    ~FibonacciSpec() override = default;
};

#endif //LED_SEGMENTS_FIBONACCIDISPLAY_H
