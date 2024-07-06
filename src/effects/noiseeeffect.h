#ifndef LED_MATRIX_DLH_NOISEEFFECT_H
#define LED_MATRIX_DLH_NOISEEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"
#include "config/variation.h"

class NoiseEffect : public Effect, public EffectFactory<NoiseEffect> {
protected:
    uint8_t noiseSpeed = random8(5, 10);

public:
    explicit NoiseEffect(
            const Section &section,
            const Mirror mirror,
            const uint8_t seed
    ) : Effect(section, mirror, seed) {
    }

    ~NoiseEffect() override = default;

    void fillArrayInternal(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror, uint8_t)> factory;
    static Variation variation;
};

#endif //LED_MATRIX_DLH_NOISEEFFECT_H
