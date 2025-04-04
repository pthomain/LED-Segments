#ifndef LED_SEGMENTS_RAINBOWEFFECT_H
#define LED_SEGMENTS_RAINBOWEFFECT_H

#include "engine/effect/Effect.h"

class RainbowEffect : public Effect, public Effect::Factory<RainbowEffect> {
public:
    explicit RainbowEffect(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
    ) override;

    String name() const override { return "Rainbow"; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_RAINBOWEFFECT_H
