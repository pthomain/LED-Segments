#ifndef LED_SEGMENTS_TRAVELEFFECT_H
#define LED_SEGMENTS_TRAVELEFFECT_H

#include "engine/effect/effect.h"

class TravelEffect : public Effect, public Effect::Factory<TravelEffect> {
public:
    explicit TravelEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    void fillArrayInternal(
            CRGB *effectArray,
            const uint16_t effectArraySize,
            const uint16_t frameIndex
    ) override;

    String name() const override { return "Travel"; }

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_TRAVELEFFECT_H
