#ifndef LED_SEGMENTS_PARTYEFFECT_H
#define LED_SEGMENTS_PARTYEFFECT_H

#include "engine/effect/effect.h"

class PartyEffect : public Effect, public Effect::Factory<PartyEffect> {
private:
    uint8_t primeA;
    uint8_t primeB;
    uint8_t scale;

public:
    explicit PartyEffect(const EffectContext &effectContext) : Effect(effectContext) {
        primeA = PRIMES.at(random8(PRIMES.size()));
        primeB = PRIMES.at(random8(PRIMES.size()));
        scale = random8(128, 192);
    }

    void fillArrayInternal(
            CRGB *effectArray,
            const uint16_t effectArraySize,
            const uint16_t frameIndex
    ) override;

    String name() const override { return "Party"; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_PARTYEFFECT_H
