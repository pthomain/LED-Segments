#include "FibonacciSpec.h"
#include "FibonacciEnums.h"

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    return getLayoutName(layoutIndex);
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
    return 1;
}

uint16_t FibonacciSpec::segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    auto pixelUnit = getPixelUnit(layoutIndex);
    auto alignment = getAlignment(layoutIndex);

    if (alignment == SPIRAL) {
        return pixelUnit == PIXEL ? PIXELS_IN_SPIRAL : NB_SPIRAL_SEGMENTS;
    } else {
        //different effect per segment, otherwise the whole display would have a single colour
        return pixelUnit == PIXEL ? 10 : NB_CONCENTRIC_SEGMENTS;
    }
}

void FibonacciSpec::applyColourToPixel(
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    uint16_t pixelIndex,
    int8_t inflexionPoint,
    CRGB *outputArray,
    const CRGB colour
) const {
    bool isEvenSegment = segmentIndex % 2 == 0;
    uint8_t threshold_2_leds;
    uint8_t threshold_3_leds;

    auto applyColourToLed = [&](uint16_t ledIndex) {
        outputArray[ledIndex] = colour;
    };

    if (getAlignment(layoutIndex) == CONCENTRIC) {
        if (isEvenSegment) {
            threshold_2_leds = 8;
            threshold_3_leds = 9;
        } else {
            threshold_2_leds = 7;
            threshold_3_leds = 9;
        }
    } else {
        threshold_2_leds = 15;
        threshold_3_leds = 18;
        int8_t inflexionAtPixel = inflexionPoint == -1 ? 0 : max(0, pixelIndex - inflexionPoint);
        uint16_t segmentOffset = (LEDS_IN_SPIRAL * (segmentIndex + inflexionAtPixel)) % nbLeds();
        uint16_t ledIndex = segmentOffset + pixelIndex;

        if (pixelIndex < threshold_2_leds) {
            applyColourToLed(ledIndex);
        } else if (pixelIndex < threshold_3_leds) {
            int8_t pixelOffset = pixelIndex - threshold_2_leds;
            applyColourToLed(ledIndex + pixelOffset);
            applyColourToLed(ledIndex + pixelOffset + 1);
        } else {
            int8_t pixelOffset = pixelIndex - threshold_2_leds + pixelIndex - threshold_3_leds;
            applyColourToLed(ledIndex + pixelOffset);
            applyColourToLed(ledIndex + pixelOffset + 1);
            applyColourToLed(ledIndex + pixelOffset + 2);
        }
    }
};

void FibonacciSpec::applyColourToSpiralSegment(
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    int8_t inflexionPoint,
    CRGB *outputArray,
    const CRGB colour
) const {
    //for each SEGMENT_IN_X, each pixel index represents a segment and the colour must be applied to the entire segment
    for (uint16_t pixelIndex = 0; pixelIndex < PIXELS_IN_SPIRAL; pixelIndex++) {
        applyColourToPixel(
            layoutIndex,
            segmentIndex,
            pixelIndex,
            inflexionPoint,
            outputArray,
            colour
        );
    }
};

void FibonacciSpec::applyColourToSpiralPixel(
    const uint16_t layoutIndex,
    const uint16_t pixelIndex,
    int8_t inflexionPoint,
    CRGB *outputArray,
    const CRGB colour
) const {
    //for each PIXEL_IN_X, the colour must be applied to the same pixel index for each segment
    for (uint16_t segmentIndex = 0; segmentIndex < NB_SPIRAL_SEGMENTS; segmentIndex++) {
        applyColourToPixel(
            layoutIndex,
            segmentIndex,
            pixelIndex,
            inflexionPoint,
            outputArray,
            colour
        );
    }
};

void FibonacciSpec::setColour(
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    const uint16_t pixelIndex,
    const uint16_t frameIndex,
    CRGB *outputArray,
    const CRGB colour
) const {
    auto pixelUnit = getPixelUnit(layoutIndex);
    auto alignment = getAlignment(layoutIndex);
    auto inflexion = getInflexion(layoutIndex);
    int8_t inflexionValue;

    switch (inflexion) {
        case INFLEXION_NONE: inflexionValue = -1;
            break;
        case INFLEXION_AT_20: inflexionValue = 20;
            break;
        case INFLEXION_DYNAMIC: inflexionValue = frameIndex % 27;
            break;
        default: inflexionValue = pixelIndex % 2 == 0 ? 1 : -1;
    }

    auto applyColourToSpiralSegmentWithInflexion = [&](int8_t inflexionPoint) {
        applyColourToSpiralSegment(
            layoutIndex,
            pixelIndex,
            inflexionPoint,
            outputArray,
            colour
        );
    };

    auto applyColourToSpiralPixelWithInflexion = [&](int8_t inflexionPoint) {
        applyColourToSpiralPixel(
            layoutIndex,
            pixelIndex,
            inflexionPoint,
            outputArray,
            colour
        );
    };

    if (alignment == SPIRAL) {
        if (pixelUnit == PIXEL) {
            applyColourToSpiralPixelWithInflexion(inflexionValue);
        } else {
            applyColourToSpiralSegmentWithInflexion(inflexionValue);
        }
    }
}
