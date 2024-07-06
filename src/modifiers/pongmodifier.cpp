#include "effects/effect.h"
#include "modifier.h"
#include "pongmodifier.h"

std::function<Effect *(const Section &, const Mirror, uint8_t)> PongModifier::factory = [](
        const Section &section,
        const Mirror mirror,
        uint8_t seed
) -> Effect * {
    return new PongModifier(section, mirror, seed);
};

void PongModifier::fillAlphaArray() {
    auto offPixel = 0;
    if (seed % 2 == 0)offPixel = 32;

    for (int i = 0; i < arraySize; ++i) {
        alphaArray[i] = i == position ? 255 : offPixel;
    }

    isReversed ? --position : ++position;

    if (position == 0 || position == arraySize - 1) {
        isReversed = !isReversed;
    }
};
