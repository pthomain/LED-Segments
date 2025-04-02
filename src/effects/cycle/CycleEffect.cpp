#include "CycleEffect.h"
#include <functional>

EffectFactory CycleEffect::factory = [](
        const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::unique_ptr<Effect>(new CycleEffect(effectContext));
};

void CycleEffect::fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
) {
    CRGB colour = CRGB::Black;
    for (uint16_t i = 0; i < effectArraySize; i++) {
        switch (effectContext.effectIndex % 3) {
            case 0:
                colour = CRGB::Red;
                break;

            case 1:
                colour = CRGB::Green;
                break;

            default:
                colour = CRGB::Blue;
                break;
        }

        effectArray[i] = colour;
    }
};
