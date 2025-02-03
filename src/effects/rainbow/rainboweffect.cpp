#include "rainboweffect.h"

EffectFactory RainbowEffect::factory = [](
        const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::unique_ptr<Effect>(new RainbowEffect(effectContext));
};

void RainbowEffect::fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
) {
    fill_rainbow(effectArray, effectArraySize, 0, 255 / effectArraySize);
};
