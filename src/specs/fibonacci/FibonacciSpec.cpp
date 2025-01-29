#include "FibonacciSpec.h"
#include "FibonacciEnums.h"

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    return getLayoutName(variations[layoutIndex]);
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
    return 1;
}

uint16_t FibonacciSpec::segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    auto variation = variations[layoutIndex];
    auto pixelUnit = getPixelUnit(variation);
    auto alignment = getAlignment(variation);

    if (alignment == SPIRAL) {
        return pixelUnit == PIXEL ? NB_PIXELS_IN_SPIRAL : NB_SPIRAL_SEGMENTS;
    } else {
        //different effect per segment, otherwise the whole display would have a single colour
        return pixelUnit == PIXEL ? NB_PIXELS_IN_CONCENTRIC : NB_CONCENTRIC_SEGMENTS;
    }
}

void FibonacciSpec::applyColourToPixel(
        const uint16_t variation,
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
        uint16_t remappedIndex = ledIndex;  //segments 0 and 1 are swapped
        if (ledIndex >= 0 && ledIndex < 27) {
            remappedIndex = ledIndex + 27;
        } else if (ledIndex >= 27 && ledIndex < 54) {
            remappedIndex = ledIndex - 27;
        }
        outputArray[remappedIndex] = colour;
    };

    if (getAlignment(variation) == CONCENTRIC) {
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

//        int8_t inflexionAtPixel = inflexionPoint == -1 ? 0 : max(0, pixelIndex - inflexionPoint);
//        uint16_t segmentOffset = (LEDS_IN_SPIRAL * (segmentIndex + inflexionAtPixel)) % nbLeds();

        auto direction = getDirection(variation);
        uint16_t directedSegmentIndex;
        uint16_t segmentOffset;

        if (direction == COUNTER_CLOCKWISE) {
            directedSegmentIndex = (segmentIndex + pixelIndex) % NB_SPIRAL_SEGMENTS;
        } else {
            directedSegmentIndex = segmentIndex;
        }

        segmentOffset = (LEDS_IN_SPIRAL * directedSegmentIndex) % nbLeds();
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

void FibonacciSpec::applyColourToPixelUnit(
        const uint16_t variation,
        const uint16_t pixelUnitIndex,
        int8_t inflexionPoint,
        CRGB *outputArray,
        const CRGB colour
) const {
    if (getPixelUnit(variation) == PIXEL) {
        //the colour must be applied to the same pixel index for each segment
        for (uint16_t segmentIndex = 0; segmentIndex < NB_SPIRAL_SEGMENTS; segmentIndex++) {
            applyColourToPixel(
                    variation,
                    segmentIndex,
                    pixelUnitIndex, //pixel index
                    inflexionPoint,
                    outputArray,
                    colour
            );
        }
    } else {
        //each pixel index represents a segment and the colour must be applied to the entire segment
        for (uint16_t pixelIndex = 0; pixelIndex < NB_PIXELS_IN_SPIRAL; pixelIndex++) {
            applyColourToPixel(
                    variation,
                    pixelUnitIndex,  //segment index
                    pixelIndex,
                    inflexionPoint,
                    outputArray,
                    colour
            );
        }
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
    auto variation = variations[layoutIndex];
    auto inflexion = getInflexion(variation);
    int8_t inflexionPoint;

    switch (inflexion) {
        case INFLEXION_NONE:
            inflexionPoint = -1;
            break;
        case INFLEXION_AT_20:
            inflexionPoint = 20;
            break;
//        case INFLEXION_DYNAMIC:
//            inflexionPoint = frameIndex % 27;
//            break;
        default:
            inflexionPoint = pixelIndex % 2 == 0 ? 1 : -1;
    }

    applyColourToPixelUnit(
            variation,
            pixelIndex, //optimisation for spirals, each segment is considered a pixel
            inflexionPoint,
            outputArray,
            colour
    );
}
