#include "partyeffect.h"

std::function<Effect *(const EffectContext &effectContext)> PartyEffect::factory = [](
        const EffectContext &effectContext
) -> Effect * {
    return new PartyEffect(effectContext);
};

void PartyEffect::fillArrayInternal(CRGB *targetArray) {
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

    for (int i = 0; i < arraySize; i++) {
        if (i % 3 == 0) {
            targetArray[i] = CRGB::Red;
        } else if (i % 3 == 1) {
            targetArray[i] = CRGB::Green;
        } else {
            targetArray[i] = CRGB::Blue;
        }
    }

};
