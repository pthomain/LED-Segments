#ifndef LED_SEGMENTS_NOISEEFFECT_H
#define LED_SEGMENTS_NOISEEFFECT_H

#include <vector>
#include <functional>
#include "effects/effect.h"
#include "colorutils.h"

class NoiseEffect : public Effect, public EffectFactory<NoiseEffect> {
protected:
    uint8_t noiseSpeed = random8(5, 10);

public:
    explicit NoiseEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    ~NoiseEffect() override = default;

    void fillArrayInternal(CRGB *targetArray) override;

    static std::function<Effect *(const EffectContext &effectContext)> factory;
};

#endif //LED_SEGMENTS_NOISEEFFECT_H
