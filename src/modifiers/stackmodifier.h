#ifndef LED_MATRIX_DLH_STACKMODIFIER_H
#define LED_MATRIX_DLH_STACKMODIFIER_H

#include "effects/effect.h"
#include "modifiers/modifier.h"

class StackModifier : public Modifier, public ModifierFactory<StackModifier> {
public:
    explicit StackModifier(
            const Section &section,
            const Mirror mirror,
            uint8_t seed
    ) : Modifier(section, mirror, seed) {}

    ~StackModifier() override = default;

    void fillAlphaArray() override;

    static std::function<Effect *(const Section &, const Mirror, uint8_t)> factory;
};

#endif //LED_MATRIX_DLH_STACKMODIFIER_H
