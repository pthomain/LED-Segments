#include "partyeffect.h"

EffectFactory PartyEffect::factory = [](
        const EffectContext &effectContext
) -> Effect* {
    return new PartyEffect(effectContext);
};

void PartyEffect::fillArray(CRGB *effectArray, uint16_t effectArraySize) {
    uint16_t beatA = beatsin16(11, 0, 255);
    uint16_t beatB = beatsin16(37, 0, 255);

    int startIndex = (beatA + beatB) / 2;

//    if (effectContext.() % 3 == 0) {
//        startIndex = iteration; //just cycle linearly
//    }

    fill_palette(
            effectArray,
            effectArraySize,
            startIndex,
            scale,
            effectContext.palette,
            255,
            LINEARBLEND
    );

//    fill_palette(effectArray, effectArraySize, 0, scale, RainbowColors_p, 255, LINEARBLEND);
};
