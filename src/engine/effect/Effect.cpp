#include "Effect.h"

//TODO must be offset by the cumulative number of LEDs for the pixel at pos cycleStep
void Effect::fillArray(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t frameIndex
) {
    uint8_t delta = max(1, 255 / effectArraySize); //TODO use percent
    if (frameIndex % cycleSpeed == 0) {
        linearCycleStep = unsignedModulo(linearCycleStep + delta, 255);
        if (circularCycleStep == 0 || circularCycleStep >= 255) isCycleReversed = !isCycleReversed;
        circularCycleStep += isCycleReversed ? delta : -delta;
    }
    fillArrayInternal(effectArray, effectArraySize, frameIndex);
};