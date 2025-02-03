#include "FibonacciSpec.h"
#include "FibonacciEnums.h"

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    return getLayoutName(variations[layoutIndex]);
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    auto variation = variations[layoutIndex];
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
    //direction, inflexion and alignment are not meaningful when using PIXEL with this optimisation
    //so they should be filtered out, unless I decide to add a different effect per segment
    return 1;
}

uint16_t FibonacciSpec::segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    auto variation = variations[layoutIndex];
    auto pixelUnit = getPixelUnit(variation);
    auto alignment = getAlignment(variation);

    if (alignment == SPIRAL) {
        return pixelUnit == PIXEL ? NB_SPIRAL_PIXELS : NB_SPIRAL_SEGMENTS;
    } else {
        //different effect per segment, otherwise the whole display would have a single colour
        return pixelUnit == PIXEL ? NB_RADIAL_PIXELS : NB_RADIAL_SEGMENTS;
    }
}

std::pair<uint16_t, uint8_t> FibonacciSpec::radialToSpiralIndex(
        const Direction direction,
        const uint8_t segmentIndex,
        const uint8_t radialIndex
) const {
    bool isEvenSegment = segmentIndex % 2 == 0;
    uint8_t segmentOffset = isEvenSegment ? segmentIndex / 2 : (segmentIndex + 1) / 2;
    uint16_t segmentStart = unsignedModulo(NB_LEDS_IN_SPIRAL * (segmentOffset + radialIndex), nbLeds());
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
        uint16_t pixelIndex,
        int8_t inflexionPoint,
        CRGB *outputArray,
        const CRGB colour
) const {
    auto applyColourToLed = [&](uint16_t ledIndex) {
        uint16_t remappedIndex = ledIndex;  //segments 0 and 1 are swapped
        if (ledIndex >= 0 && ledIndex < 27) {
            remappedIndex = ledIndex + 27;
        } else if (ledIndex >= 27 && ledIndex < 54) {
            remappedIndex = ledIndex - 27;
        }
        outputArray[remappedIndex] = colour;
    };

    auto direction = getDirection(variation);
    uint16_t directedSegmentIndex = segmentIndex;
    if (getAlignment(variation) == SPIRAL) {
        //TODO check other direction
        if (direction == COUNTER_CLOCKWISE || inflexionPoint >= pixelIndex) {
            directedSegmentIndex = unsignedModulo(
                    segmentIndex + pixelIndex - (NB_SPIRAL_PIXELS - inflexionPoint),
                    NB_SPIRAL_SEGMENTS
            );
        }
    }

    uint16_t segmentStart;
    uint8_t spiralPixelIndex;
    if (getAlignment(variation) == RADIAL) {
        auto segmentStartAndSpiralLedIndex = radialToSpiralIndex(direction, directedSegmentIndex, pixelIndex);
        segmentStart = segmentStartAndSpiralLedIndex.first;
        spiralPixelIndex = segmentStartAndSpiralLedIndex.second;
    } else {
        spiralPixelIndex = pixelIndex;
        segmentStart = (NB_LEDS_IN_SPIRAL * directedSegmentIndex) % nbLeds();
    }

    uint16_t ledIndex = segmentStart + spiralPixelIndex + getLedOffset(spiralPixelIndex);

    for (uint8_t padding = 0; padding <= getLedPadding(spiralPixelIndex); padding++) {
        applyColourToLed(ledIndex + padding);
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
        int nbSegments = getAlignment(variation) == SPIRAL ? NB_SPIRAL_SEGMENTS : NB_RADIAL_SEGMENTS;
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
                    pixelUnitIndex,  //segment index
                    pixelIndex,
                    inflexionPoint,
                    outputArray,
                    colour
            );
        }
    }
};

int8_t inflexionPoint = -1;

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

    switch (inflexion) {
        case INFLEXION_AT_16:
            if (inflexionPoint == -1)inflexionPoint = 17;
            break;
        case INFLEXION_NONE:
        default:
            inflexionPoint = -1;
            break;
    }

//    if (inflexionPoint != -1 && frameIndex % 10 == 0) {
//        inflexionPoint = (inflexionPoint + 1) % NB_SPIRAL_PIXELS;
//    }

    applyColourToPixelUnit(
            variation,
            pixelIndex,//optimisation for spirals, each segment is considered a pixel
            inflexionPoint,
            outputArray,
            colour
    );
}
