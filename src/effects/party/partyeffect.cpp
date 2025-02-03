#include "partyeffect.h"

EffectFactory PartyEffect::factory = [](
        const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::unique_ptr<Effect>(new PartyEffect(effectContext));
};

void PartyEffect::fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
) {
    uint16_t beatA = beatsin16(11, 0, 255);
    uint16_t beatB = beatsin16(37, 0, 255);
    uint16_t startIndex = (beatA + beatB) / 2;

    fill_palette(
            effectArray,
            effectArraySize,
            startIndex,
            scale,
            effectContext.palette,
            255,
            LINEARBLEND
    );
};
