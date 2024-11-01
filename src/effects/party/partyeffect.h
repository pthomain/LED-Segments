#ifndef LED_SEGMENTS_PARTYEFFECT_H
#define LED_SEGMENTS_PARTYEFFECT_H

#include <vector>
#include <functional>
#include "effects/effect.h"
#include "utils/utils.h"
#include "colorutils.h"

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
};

#endif //LED_SEGMENTS_PARTYEFFECT_H
