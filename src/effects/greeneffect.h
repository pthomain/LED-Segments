#ifndef LED_MATRIX_DLH_GREENEFFECT_H
#define LED_MATRIX_DLH_GREENEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"

class GreenEffect : public Effect, public EffectFactory<GreenEffect> {
private:

public:
    explicit GreenEffect(
        const Section &section,
        const Mirror mirror
    ) : Effect(section, mirror) {
    }

    ~GreenEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror)> factory;
};

#endif //LED_MATRIX_DLH_GREENEFFECT_H
