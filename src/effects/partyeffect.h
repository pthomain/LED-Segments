#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include "effect.h"

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {

public:
    explicit PartyEffect(const Modifier *modifier) : Effect(modifier) {}

    ~PartyEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static const std::function<Effect *(const Modifier *)> factory;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
