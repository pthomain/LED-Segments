#ifndef LED_MATRIX_DLH_PONGMODIFIER_H
#define LED_MATRIX_DLH_PONGMODIFIER_H

#include "effects/effect.h"
#include "modifiers/modifier.h"

class PongModifier : public Modifier, public ModifierFactory<PongModifier> {

private:
    bool isReversed = false;
    int position = 0;

public:
    explicit PongModifier(const EffectContext &effectContext) : Modifier(effectContext) {}

    ~PongModifier() override = default;

    void fillAlphaArray() override;

    static std::function<Effect *(const EffectContext &effectContext)> factory;
};

#endif //LED_MATRIX_DLH_PONGMODIFIER_H
