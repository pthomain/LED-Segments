#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include <vector>
#include <functional>
#include "effects/effect.h"
#include "colorutils.h"

class NoiseEffect : public Effect, public Effect::Factory<NoiseEffect> {
protected:
    uint8_t noiseSpeed = random8(5, 10);

public:
    explicit NoiseEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    void fillArray(CRGB *effectArray, uint16_t effectArraySize) override;

    static EffectFactory factory;
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
