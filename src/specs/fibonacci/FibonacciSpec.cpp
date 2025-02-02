#include "FibonacciSpec.h"
#include "FibonacciEnums.h"

String FibonacciSpec::layoutName(const uint16_t layoutIndex) const {
    return getLayoutName(variations[layoutIndex]);
}

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    auto variation = variations[layoutIndex];
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
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
        const uint8_t segmentIndex,
        const uint8_t radialIndex
) const {
    bool isEvenSegment = segmentIndex % 2 == 0;
    int i = isEvenSegment ? segmentIndex / 2 : (segmentIndex + 1) / 2;
    uint16_t segmentStart = unsignedModulo(NB_LEDS_IN_SPIRAL * (i + radialIndex), nbLeds());
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

    uint16_t segmentStart;
    uint8_t spiralPixelIndex;

    if (getAlignment(variation) == RADIAL) {
        auto segmentStartAndSpiralLedIndex = radialToSpiralIndex(segmentIndex, pixelIndex);
        segmentStart = segmentStartAndSpiralLedIndex.first;
        spiralPixelIndex = segmentStartAndSpiralLedIndex.second;
    } else {
        spiralPixelIndex = pixelIndex;

        auto direction = getDirection(variation);
        uint16_t directedSegmentIndex;
        if (direction == COUNTER_CLOCKWISE) {
            directedSegmentIndex = (segmentIndex + pixelIndex) % NB_SPIRAL_SEGMENTS;
        } else {
            directedSegmentIndex = segmentIndex;
        }

        segmentStart = (NB_LEDS_IN_SPIRAL * directedSegmentIndex) % nbLeds();
    }

    uint16_t ledIndex = segmentStart + spiralPixelIndex + getLedOffset(spiralPixelIndex);

    for (uint8_t padding = 0; padding <= getLedPadding(spiralPixelIndex); padding++) {
        applyColourToLed(ledIndex + padding);
    }
};

uint16_t segmentRotation = 0;

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
        uint8_t segmentModulo = 1;

//        switch (getSegmentModulo(variation)) {
//            case MODULO_2:
//                segmentModulo = 2;
//                break;
//            case MODULO_3:
//                segmentModulo = 3;
//                break;
//            case MODULO_4:
//                segmentModulo = 4;
//                break;
//            case MODULO_6:
//                segmentModulo = 6;
//                break;
//            case MODULO_12:
//                segmentModulo = 12;
//                break;
//                case MODULO_0:
//            default:
//                segmentModulo = 0;
//                break;
//        }

        //each pixel index represents a segment and the colour must be applied to the entire segment
        int nbPixels = getAlignment(variation) == SPIRAL ? NB_SPIRAL_PIXELS : NB_RADIAL_PIXELS;
        for (uint16_t pixelIndex = 0; pixelIndex < nbPixels; pixelIndex++) {
            applyColourToPixel(
                    variation,
                    pixelUnitIndex,  //segment index
                    pixelIndex,
                    inflexionPoint,
                    outputArray,
                    colour// (pixelUnitIndex + segmentRotation) % segmentModulo == 0 ? 0 : colour
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
        default:
            inflexionPoint = pixelIndex % 2 == 0 ? 1 : -1;
    }

    if (getPixelUnit(layoutIndex) == SEGMENT && frameIndex % 5 == 0) {
        segmentRotation = unsignedModulo(
                segmentRotation + (getDirection(layoutIndex) == CLOCKWISE ? -1 : 1),
                NB_SPIRAL_SEGMENTS
        );
//        Serial.println("segmentRotation: " + String(segmentRotation));
    }

    applyColourToPixelUnit(
            variation,
            pixelIndex,//optimisation for spirals, each segment is considered a pixel
            inflexionPoint,
            outputArray,
            colour
    );
}
