#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include <vector>
#include "effect.h"
#include "utils/utils.h"
#include "colorutils.h"
#include "config/variation.h"

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {
private:
    uint8_t primeA;
    uint8_t primeB;

public:
    explicit PartyEffect(const EffectContext &effectContext) : Effect(effectContext) {
        primeA = PRIMES.at(random8(PRIMES.size()));
        primeB = PRIMES.at(random8(PRIMES.size()));
    }

    ~PartyEffect() override = default;

    void fillArrayInternal(CRGB *targetArray) override;

    static std::function<Effect *(const EffectContext &effectContext)> factory;
    static Variation variation;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
