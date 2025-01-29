#ifndef LED_SEGMENTS_PARTYEFFECT_H
#define LED_SEGMENTS_PARTYEFFECT_H

#include <vector>
#include "engine/effect/effect.h"

class PartyEffect : public Effect, public Effect::Factory<PartyEffect> {
private:
    uint8_t primeA;
    uint8_t primeB;

public:
    explicit PartyEffect(const EffectContext &effectContext) : Effect(effectContext) {
        primeA = PRIMES.at(random8(PRIMES.size()));
        primeB = PRIMES.at(random8(PRIMES.size()));
    }

    void fillArray(
            CRGB *effectArray,
            const uint16_t effectArraySize,
            const uint16_t frameIndex
    ) override;

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_PARTYEFFECT_H
