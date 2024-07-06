#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include <vector>
#include "effect.h"
#include "utils/utils.h"
#include "colorutils.h"
#include "config/variation.h"

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {
private:
    CRGBPalette16 palette;
    uint8_t paletteIndex;
    uint8_t primeA;
    uint8_t primeB;

public:
    explicit PartyEffect(
            const Section &section,
            const Mirror mirror,
            const uint8_t seed
    ) : Effect(section, mirror, seed) {
        palette = PALETTES[seed % PALETTES.size()];
        paletteIndex = (paletteIndex + 1) % PALETTES.size();
        primeA = PRIMES.at(random8(PRIMES.size()));
        primeB = PRIMES.at(random8(PRIMES.size()));
    }

    ~PartyEffect() override = default;

    void fillArrayInternal(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror, uint8_t)> factory;
    static Variation variation;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
