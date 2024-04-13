#include "effects/effect.h"
#include "modifier.h"
#include "pongmodifier.h"

std::function<Effect *(const Section &, const Mirror)> PongModifier::factory = [](
        const Section &section,
        const Mirror mirror
) -> Effect * {
    return new PongModifier(section, mirror);
};

void PongModifier::fillAlphaArray() {
    for (int i = 0; i < arraySize; ++i) {
        alphaArray[i] = i == position ? 255 : 0;
    }

    isReversed ? --position : ++position;

    if (position == 0 || position == arraySize - 1) {
        isReversed = !isReversed;
    }
};