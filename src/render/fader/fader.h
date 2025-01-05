#ifndef LED_SEGMENTS_FADER_H
#define LED_SEGMENTS_FADER_H

#include "FastLED.h"
#include "config.h"
#include "effects/effect.h"
#include "render/renderer.h"
#include "displayspec/displayspec.h"
#include "memory"

class Fader : public Renderer {

private:
    constexpr static const float TRANSITION = (float) TRANSITION_DURATION_IN_FRAMES;

    Renderer *firstRenderer;
    Renderer *secondRenderer;
    CRGB *blendingArray;

    bool isFirstEffectRendering = false;
    float crossFadeStep = 0;

public :

    explicit Fader(std::shared_ptr<DisplaySpec> displaySpec);

    void changeEffect(std::unique_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    ~Fader() override {
        delete[] blendingArray;
        delete firstRenderer;
        delete secondRenderer;
    }
};

#endif //LED_SEGMENTS_FADER_H
