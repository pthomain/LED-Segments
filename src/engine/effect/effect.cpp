#include "effect.h"

//TODO check logic for bounce
void Effect::shiftArrayValues(CRGB *array, uint16_t arraySize, uint16_t shift) {
    if (arraySize == 0) return;
    shift = unsignedModulo(shift, arraySize);
    if (shift == 0) return;

    uint16_t nbMovedValues = 0;
    for (uint16_t startIndex = 0; nbMovedValues < arraySize; ++startIndex) {
        CRGB previousValue = array[startIndex];
        uint16_t currentIndex = startIndex;

        do {
            uint16_t nextIndex = unsignedModulo(currentIndex + shift, arraySize);
            CRGB temp = array[nextIndex];
            array[nextIndex] = previousValue;
            previousValue = temp;
            currentIndex = nextIndex;
            nbMovedValues++;
        } while (startIndex != currentIndex);
    }
}

void Effect::fillArray(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t frameIndex
) {
    fillArrayInternal(effectArray, effectArraySize, frameIndex);

    if (frameIndex % 2 == 0) { //TODO handle FPS
        if (effectContext.cycle == WRAP) {
            start = unsignedModulo(start + speed, 255);
        } else {
            if (start == 0 || start == 255) speed = -speed;
            start += speed;
        }
    }

    shiftArrayValues(effectArray, effectArraySize, start);
};