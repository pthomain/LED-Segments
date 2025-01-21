#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include <functional>
#include "effect/effect.h"

class NoiseEffect : public Effect, public Effect::Factory<NoiseEffect> {
protected:
    uint8_t noiseSpeed = random8(5, 10);

public:
    explicit NoiseEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    void fillArray(
            CRGB *effectArray,
            const uint16_t effectArraySize,
            const uint16_t frameIndex
    ) override;

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
