#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include "effect.h"

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {

public:
    explicit PartyEffect(
            const Section &section,
            const Mirror mirror
    ) : Effect(section, mirror) {}

    ~PartyEffect() = default;

    void fillArray(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror)> factory;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
