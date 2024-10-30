#ifndef LED_SEGMENTS_SWIPEEFFECT_H
#define LED_SEGMENTS_SWIPEEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"
#include "config/variation.h"

class SwipeEffect : public Effect, public EffectFactory<SwipeEffect> {
protected:
    uint8_t SwipeSpeed = random8(5, 10);


public:
    explicit SwipeEffect(const EffectContext &effectContext) : Effect(effectContext) {}

    ~SwipeEffect() override = default;

    void fillArrayInternal(CRGB *targetArray) override;

    static std::function<Effect *(const EffectContext &effectContext)> factory;
    static Variation variation;
};

#endif //LED_SEGMENTS_SWIPEEFFECT_H
