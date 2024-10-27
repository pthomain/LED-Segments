#include "partyeffect.h"
#include "modifiers/pongmodifier.h"
#include "config/variation.h"

std::function<Effect *(const EffectContext &effectContext)> PartyEffect::factory = [](
        const EffectContext &effectContext
) -> Effect * {
    return new PartyEffect(effectContext);
};

void PartyEffect::fillArrayInternal(CRGB *targetArray) {
    uint16_t beatA = beatsin16(11, 0, 255);
    uint16_t beatB = beatsin16(37, 0, 255);

    int startIndex = (beatA + beatB) / 2;

    if (effectContext.effectIteration % 3 == 0) {
        startIndex = iteration; //just cycle linearly
    }

    fill_palette(
            targetArray,
            arraySize,
            startIndex,
            scale,
            palette,
            255,
            LINEARBLEND
    );
};

Variation PartyEffect::variation = Variation(
        {
                std::make_pair(SCOPE_WORD, UNIT_LETTER),
                std::make_pair(SCOPE_WHOLE, UNIT_WORD)
        },
        {MIRROR_NONE},
        {&PongModifier::factory}
);