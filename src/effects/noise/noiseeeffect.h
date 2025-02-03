#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include "engine/effect/effect.h"

class NoiseEffect : public Effect, public Effect::Factory<NoiseEffect> {
protected:
    uint8_t noiseSpeed = random8(5, 10);

public:
    explicit NoiseEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    void fillArrayInternal(
            CRGB *effectArray,
            const uint16_t effectArraySize,
            const uint16_t frameIndex
    ) override;

    String name() const override { return "Noise"; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
