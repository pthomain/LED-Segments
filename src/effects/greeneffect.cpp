#include "greeneffect.h"

std::function<Effect *(const Section &, const Mirror)> GreenEffect::factory = [](
        const Section &section,
        const Mirror mirror
) -> Effect * {
    return new GreenEffect(section, mirror);
};

void GreenEffect::fillArray(CRGB *targetArray) {
    for (int i = 0; i < arraySize; i++) {
        targetArray[i] = CRGB::Green;
    }
};
