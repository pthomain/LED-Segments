#include "redeffect.h"

std::function<Effect *(const Section &, const Mirror)> RedEffect::factory = [](
        const Section &section,
        const Mirror mirror
) -> Effect * {
    return new RedEffect(section, mirror);
};

void RedEffect::fillArray(CRGB *targetArray) {
    for (int i = 0; i < arraySize; i++) {
        targetArray[i] = CRGB::Red;
    }
};
