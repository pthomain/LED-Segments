#include "RainbowEffect.h"

EffectFactory RainbowEffect::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<RainbowEffect>(effectContext);
};

void RainbowEffect::fillArrayInternal(
    CRGB *effectArray,
    const uint16_t effectArraySize,
    const uint16_t frameIndex
) {
    const uint8_t deltaHue = max(1, 255 / effectArraySize);
    const boolean step = linearCycleStep;
    fill_rainbow(effectArray, effectArraySize, step, deltaHue);
};
