#include "effects/effect.h"
#include "modifier.h"
#include "pongmodifier.h"

std::function<Effect *(const EffectContext &effectContext)> PongModifier::factory = [](
        const EffectContext &effectContext
) -> Effect * {
    return new PongModifier(effectContext);
};

void PongModifier::fillAlphaArray() {
    auto offPixel = 0;
    if (effectContext.seed % 2 == 0)offPixel = 32;

    for (int i = 0; i < arraySize; ++i) {
        alphaArray[i] = i == position ? 255 : offPixel;
    }

    isReversed ? --position : ++position;

    if (position == 0 || position == arraySize - 1) {
        isReversed = !isReversed;
    }
};
