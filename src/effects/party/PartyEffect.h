#ifndef LED_SEGMENTS_PARTYEFFECT_H
#define LED_SEGMENTS_PARTYEFFECT_H

#include "engine/effect/effect.h"

class PartyEffect : public Effect, public Effect::Factory<PartyEffect> {
    const uint8_t primeA = PRIMES.at(random8(PRIMES.size()));
    const uint8_t primeB = PRIMES.at(random8(PRIMES.size()));
    const uint8_t scale = random8(128, 192);

public:
    explicit PartyEffect(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
    ) override;

    String name() const override { return "Party"; }
    EffectType type() const override { return EFFECT; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_PARTYEFFECT_H
