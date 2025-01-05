#include "TestSpec.h"

uint16_t TestSpec::nbSegments(const uint16_t layoutIndex) const {
    switch (layoutIndex) {
        case 0:
            return 32;
        default:
            return 0;
    }
}

uint16_t TestSpec::segmentSize(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex
) const {
    switch (layoutIndex) {
        case 0:
            return 8;
        default:
            return 0;
    }
}

void TestSpec::setColour(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        CRGB *outputArray,
        CRGB colour
) const {
    switch (layoutIndex) {
        case 0:
            setColourForSimpleRows(segmentIndex, pixelIndex, outputArray, colour);
            break;
        default:
            break;
    }
}

void TestSpec::setColourForSimpleRows(
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        CRGB *outputArray,
        CRGB colour
) const {
    auto mappedIndex = segmentIndex % 2 == 0
                       ? segmentIndex * 8 + pixelIndex
                       : (segmentIndex + 1) * 8 - 1 - pixelIndex;
    outputArray[mappedIndex] = colour;
}