#include "partyeffect.h"

EffectFactory PartyEffect::factory = [](
        const EffectContext &effectContext
) -> std::shared_ptr<Effect> {
    return std::make_shared<PartyEffect>(effectContext);
};

void PartyEffect::fillArray(CRGB *effectArray, uint16_t effectArraySize) {
//    uint16_t beatA = beatsin16(11, 0, 255);
//    uint16_t beatB = beatsin16(37, 0, 255);
//
//    int startIndex = (beatA + beatB) / 2;
//
//    if (effectContext.iteration() % 3 == 0) {
//        startIndex = iteration; //just cycle linearly
//    }
//
//    fill_palette(
//            targetArray,
//            arraySize,
//            startIndex,
//            scale,
//            palette,
//            255,
//            LINEARBLEND
//    );

    for (int i = 0; i < effectArraySize; i++) {
        if (i % 3 == 0) {
            effectArray[i] = CRGB::Red;
        } else if (i % 3 == 1) {
            effectArray[i] = CRGB::Green;
        } else {
            effectArray[i] = CRGB::Blue;
        }
    }
};
