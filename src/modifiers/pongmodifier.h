#ifndef LED_MATRIX_DLH_PONGMODIFIER_H
#define LED_MATRIX_DLH_PONGMODIFIER_H

#include "effects/effect.h"

class PongModifier : public Modifier, public ModifierFactory<PongModifier> {

private:
    bool isReversed = false;
    int position = 0;

public:
    explicit PongModifier(
            const Section &section,
            const Mirror mirror
    ) : Modifier(section, mirror) {}

    ~PongModifier() override = default;

    void setAlphaArray() override;

    static const std::function<Modifier *(const Section &, const Mirror)> factory;
};

#endif //LED_MATRIX_DLH_PONGMODIFIER_H
