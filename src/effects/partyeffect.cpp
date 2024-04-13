#include "partyeffect.h"

 std::function<Effect *(const Section &, const Mirror)> PartyEffect::factory = [](
        const Section &section,
        const Mirror mirror
) -> Effect * {
    return new PartyEffect(section, mirror);
};

void PartyEffect::fillArray(CRGB *targetArray) {
    uint16_t beatA = beatsin16(19, 0, 255);
    uint16_t beatB = beatsin16(41, 0, 255);

//            PartyColors_p,a
//    ForestColors_p
//    CloudColors_p
//    LavaColors_p
//    OceanColors_p

    fill_palette(
            targetArray,
            arraySize,
            (beatA + beatB) / 2,
            10,
            PartyColors_p,
            255,
            LINEARBLEND
    );
};