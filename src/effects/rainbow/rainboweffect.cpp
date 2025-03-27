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
    uint8_t deltaHue = max(1, 255 / effectArraySize);
    fill_rainbow(effectArray, effectArraySize, 0, deltaHue);
};
