#ifndef LED_SEGMENTS_CYCLEEFFECT_H
#define LED_SEGMENTS_CYCLEEFFECT_H

#include "engine/effect/Effect.h"

class CycleEffect : public Effect, public Effect::Factory<CycleEffect> {
public:
    explicit CycleEffect(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
    ) override;

    String name() const override { return "Cycle"; }
    EffectType type() const override { return EFFECT; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_CYCLEEFFECT_H
