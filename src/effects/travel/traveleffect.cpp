#include "traveleffect.h"

EffectFactory TravelEffect::factory = [](
        const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::unique_ptr<Effect>(new TravelEffect(effectContext));
};

void TravelEffect::fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
) {
    if (frameIndex % 5 == 0) start = ++start % effectArraySize;

    for (uint16_t index = 0; index < effectArraySize; index++) {
        effectArray[index] = index == start ? CRGB::Red : 0;
    }
};
