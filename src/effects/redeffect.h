#ifndef LED_MATRIX_DLH_REDEFFECT_H
#define LED_MATRIX_DLH_REDEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"

class RedEffect : public Effect, public EffectFactory<RedEffect> {
private:

public:
    explicit RedEffect(
        const Section &section,
        const Mirror mirror
    ) : Effect(section, mirror) {
    }

    ~RedEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror)> factory;
};

#endif //LED_MATRIX_DLH_REDEFFECT_H
