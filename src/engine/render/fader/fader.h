#ifndef LED_SEGMENTS_FADER_H
#define LED_SEGMENTS_FADER_H

#include "FastLED.h"
#include "engine/effect/effect.h"
#include "engine/render/renderer.h"
#include "engine/displayspec.h"
#include "memory"

class Fader : public Renderer {

private:
    Renderer *firstRenderer;
    Renderer *secondRenderer;
    CRGB *blendingArray;

    const uint16_t refreshRateInMillis;
    const uint16_t transitionDurationInMillis;
    const uint16_t transitionDurationInFrames =  transitionDurationInMillis / refreshRateInMillis;
    bool isFirstEffectRendering = false;
    float crossFadeStep = 0;

public :

    explicit Fader(
            const DisplaySpec &displaySpec,
            const String &name,
            const uint16_t refreshRateInMillis,
            const uint16_t transitionDurationInMillis
    );

    bool hasEffect() override;

    void changeEffect(std::unique_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    ~Fader() override {
        delete[] blendingArray;
        delete firstRenderer;
        delete secondRenderer;
    }
};

#endif //LED_SEGMENTS_FADER_H
