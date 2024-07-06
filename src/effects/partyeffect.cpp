#include "partyeffect.h"
#include "modifiers/pongmodifier.h"
#include "config/variation.h"
#include "modifiers/stackmodifier.h"

std::function<Effect *(const Section &, const Mirror, uint8_t)> PartyEffect::factory = [](
        const Section &section,
        const Mirror mirror,
        const uint8_t seed
) -> Effect * {
    return new PartyEffect(section, mirror, seed);
};

Variation PartyEffect::variation = Variation(
        ALL_SCOPES_WEIGHTED,
        ALL_MIRRORS_WEIGHTED,
        {
                nullptr,
//                &StackModifier::factory
        }
);

void PartyEffect::fillArrayInternal(CRGB *targetArray) {
    uint16_t beatA = beatsin16(11, 0, 255);
    uint16_t beatB = beatsin16(37, 0, 255);

    int startIndex = (beatA + beatB) / 2;

    if(seed % 3 == 0) {
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
