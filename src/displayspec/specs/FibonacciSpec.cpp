#include "FibonacciSpec.h"

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    switch (layoutIndex) {
        case PIXEL_IN_SPIRAL:
            return "PIXEL_IN_SPIRAL";
        case SEGMENT_IN_SPIRAL:
            return "SEGMENT_IN_SPIRAL";
        case PIXEL_IN_SPIRAL_INFLEXION_20:
            return "PIXEL_IN_SPIRAL_INFLEXION_20";
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
            return "SEGMENT_IN_SPIRAL_INFLEXION_20";
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
            return "PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC";
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
            return "SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC";
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
            return "PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN";
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
            return "SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN";
        case PIXEL_IN_SPIRAL_SYMMETRIC:
            return "PIXEL_IN_SPIRAL_SYMMETRIC";
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
            return "SEGMENT_IN_SPIRAL_SYMMETRIC";
        case PIXEL_IN_CONCENTRIC:
            return "PIXEL_IN_CONCENTRIC";
        case SEGMENT_IN_CONCENTRIC:
            return "SEGMENT_IN_CONCENTRIC";
        default:
            return "UNKNOWN";
    }
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    switch (layoutIndex) {
        case PIXEL_IN_SPIRAL:
        case PIXEL_IN_SPIRAL_INFLEXION_20:
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case PIXEL_IN_SPIRAL_SYMMETRIC:
        case SEGMENT_IN_SPIRAL:
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
        case PIXEL_IN_CONCENTRIC:
            return 1; //same effect applied to all segments

        case SEGMENT_IN_CONCENTRIC:
            return 24; //different effect per segment, otherwise the whole display would have a single colour

        default:
            return 0;
    }
}

uint16_t FibonacciSpec::segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    switch (layoutIndex) {
        case PIXEL_IN_SPIRAL:
        case PIXEL_IN_SPIRAL_INFLEXION_20:
        case PIXEL_IN_SPIRAL_INFLEXION_DYNAMIC:
        case PIXEL_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case PIXEL_IN_SPIRAL_SYMMETRIC:
            return 20;

        case SEGMENT_IN_SPIRAL:
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
            return 12;

        case PIXEL_IN_CONCENTRIC:
            return 10;

        case SEGMENT_IN_CONCENTRIC:
            return 1;

        default:
            return 0;
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

    if (layoutIndex == PIXEL_IN_CONCENTRIC || layoutIndex == SEGMENT_IN_CONCENTRIC) {
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
        uint16_t segmentOffset = (27 * (segmentIndex + inflexionAtPixel)) % nbLeds();
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
    for (uint16_t pixelIndex = 0; pixelIndex < 20; pixelIndex++) {
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
    for (uint16_t segmentIndex = 0; segmentIndex < 12; segmentIndex++) {
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

    switch (layoutIndex) {
        case PIXEL_IN_SPIRAL:
            applyColourToSpiralPixelWithInflexion(-1);
            break;
        case SEGMENT_IN_SPIRAL:
            applyColourToSpiralSegmentWithInflexion(-1);
            break;
        case SEGMENT_IN_SPIRAL_INFLEXION_20:
            applyColourToSpiralSegmentWithInflexion(20);
            break;
        case SEGMENT_IN_SPIRAL_INFLEXION_DYNAMIC:
            applyColourToSpiralSegmentWithInflexion(frameIndex % 27);
            break;
        case SEGMENT_IN_SPIRAL_INFLEXION_ODD_EVEN:
            //this only works if each segment has a different effect
            applyColourToSpiralSegmentWithInflexion(pixelIndex % 2 == 0 ? 1 : -1);
            break;
        case SEGMENT_IN_SPIRAL_SYMMETRIC:
            applyColourToSpiralSegmentWithInflexion(-1);
            break;
    }
}
