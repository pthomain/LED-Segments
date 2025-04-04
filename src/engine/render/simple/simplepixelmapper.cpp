#include "SimplePixelMapper.h"
#include <string>

void SimplePixelMapper::mapPixels(
    const String &rendererName,
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    const uint16_t segmentSize,
    const uint16_t frameIndex,
    CRGB *outputArray,
    CRGB *effectArray
) {
    for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
        displaySpec.setColour(
            layoutIndex,
            segmentIndex,
            pixelIndex,
            frameIndex,
            outputArray,
            effectArray[pixelIndex]
        );
    }
}
