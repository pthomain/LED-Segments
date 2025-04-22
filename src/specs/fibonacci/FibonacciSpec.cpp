#include "FibonacciSpec.h"
#include "FibonacciLayoutConfig.h"
#include <effects/noise/NoiseEffect.h>

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    return getLayoutName(variations()[layoutIndex]);
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
    //direction, inflexion and alignment are not meaningful when using PIXEL with this optimisation
    //so they should be filtered out, unless I decide to add a different effect per segment
    return 1;
}

uint16_t FibonacciSpec::nbPixels(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    const auto variation = variations()[layoutIndex];
    const auto pixelUnit = getPixelUnit(variation);
    const auto alignment = getAlignment(variation);

    if (alignment == SPIRAL) {
        return pixelUnit == PIXEL ? NB_SPIRAL_PIXELS : NB_SPIRAL_SEGMENTS;
    } else {
        //different effect per segment, otherwise the whole display would have a single colour
        return pixelUnit == PIXEL ? NB_RADIAL_PIXELS : NB_RADIAL_SEGMENTS;
    }
}

std::pair<uint16_t, uint8_t> FibonacciSpec::radialToSpiralIndex(
    const uint8_t segmentIndex,
    const uint8_t radialIndex
) const {
    const bool isEvenSegment = segmentIndex % 2 == 0;
    const uint8_t segmentOffset = isEvenSegment ? segmentIndex / 2 : (segmentIndex + 1) / 2;
    const uint16_t segmentStart = unsignedModulo(NB_LEDS_IN_SPIRAL * (segmentOffset + radialIndex), nbLeds());
    uint8_t spiralPixelIndex;

    if (isEvenSegment) {
        spiralPixelIndex = radialIndex * 2;
    } else {
        spiralPixelIndex = (radialIndex * 2) + 1;
    }

    return std::pair<uint16_t, uint8_t>(segmentStart, spiralPixelIndex);
}


uint8_t FibonacciSpec::getLedOffset(const uint8_t spiralPixelIndex) const {
    switch (spiralPixelIndex) {
        case 16:
            return 1;
        case 17:
            return 2;
        case 18:
            return 3;
        case 19:
            return 5;
        default:
            return 0;
    }
}

uint8_t FibonacciSpec::getLedPadding(const uint8_t spiralPixelIndex) const {
    switch (spiralPixelIndex) {
        case 15:
        case 16:
        case 17:
            return 1;
        case 18:
        case 19:
            return 2;
        default:
            return 0;
    }
}

void FibonacciSpec::applyColourToPixel(
    const uint16_t variation,
    const uint16_t segmentIndex,
    const uint16_t pixelIndex,
    const int8_t inflexionPoint,
    CRGB *outputArray,
    const CRGB colour
) const {
    const auto applyColourToLed = [&](uint16_t ledIndex) {
        uint16_t remappedIndex = ledIndex; //segments 0 and 1 are swapped
        if (ledIndex >= 0 && ledIndex < 27) {
            remappedIndex = ledIndex + 27;
        } else if (ledIndex >= 27 && ledIndex < 54) {
            remappedIndex = ledIndex - 27;
        }
        outputArray[remappedIndex] = colour;
    };

    const auto direction = getDirection(variation);
    const auto alignment = getAlignment(variation);

    uint16_t segmentStart;
    uint8_t spiralPixelIndex;
    if (alignment == RADIAL) {
        const auto segmentStartAndSpiralLedIndex = radialToSpiralIndex(segmentIndex, pixelIndex);
        segmentStart = segmentStartAndSpiralLedIndex.first;
        spiralPixelIndex = segmentStartAndSpiralLedIndex.second;
    } else {
        const auto ccwOffset = unsignedModulo(
            segmentIndex + pixelIndex - inflexionPoint,
            NB_SPIRAL_SEGMENTS
        );

        uint8_t directedSegmentIndex;
        //TODO special case with no inflexion, at the moment simple spirals are reversed
        //TODO check why travel is skipping every other segment

        if (pixelIndex > inflexionPoint) {
            directedSegmentIndex = direction == CLOCKWISE ? segmentIndex : ccwOffset;
        } else {
            directedSegmentIndex = direction == CLOCKWISE ? ccwOffset : segmentIndex;
        }

        spiralPixelIndex = pixelIndex;
        segmentStart = unsignedModulo(
            NB_LEDS_IN_SPIRAL * directedSegmentIndex,
            nbLeds()
        );
    }

    uint16_t ledIndex = segmentStart + spiralPixelIndex + getLedOffset(spiralPixelIndex);

    for (uint8_t padding = 0; padding <= getLedPadding(spiralPixelIndex); padding++) {
        applyColourToLed(ledIndex + padding);
    }
};

void FibonacciSpec::applyColourToPixelUnit(
    const uint16_t variation,
    const uint16_t pixelUnitIndex,
    const int8_t inflexionPoint,
    CRGB *outputArray,
    const CRGB colour
) const {
    if (getPixelUnit(variation) == PIXEL) {
        //the colour must be applied to the same pixel index for each segment
        uint16_t nbSegments = getAlignment(variation) == SPIRAL ? NB_SPIRAL_SEGMENTS : NB_RADIAL_SEGMENTS;
        for (uint16_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
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
        int nbPixels = getAlignment(variation) == SPIRAL ? NB_SPIRAL_PIXELS : NB_RADIAL_PIXELS;
        for (uint16_t pixelIndex = 0; pixelIndex < nbPixels; pixelIndex++) {
            applyColourToPixel(
                variation,
                pixelUnitIndex, //segment index
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
    const auto variation = variations()[layoutIndex];
    const auto inflexion = getInflexion(variation);
    int8_t inflexionPoint;

    switch (inflexion) {
        case STATIC_INFLEXION:
            inflexionPoint = 15;
            break;
        case DYNAMIC_INFLEXION:
            inflexionPoint = (frameIndex / 1) % NB_SPIRAL_PIXELS;
            break;
        case INFLEXION_NONE:
        default:
            inflexionPoint = -1;
            break;
    }

    applyColourToPixelUnit(
        variation,
        pixelIndex, //optimisation for spirals, each segment is considered a pixel
        inflexionPoint,
        outputArray,
        colour
    );
}
