#ifndef LED_SEGMENTS_FADER_H
#define LED_SEGMENTS_FADER_H

#include "FastLED.h"
#include "config.h"
#include "effects/effect.h"
#include "render/renderer.h"
#include "displayspec/displayspec.h"

class Fader : public Renderer {

private:

    Renderer *firstRenderer;
    Renderer *secondRenderer;
    CRGB *blendingArray;

    bool isFirstEffectRendering = false;
    float crossFadeMax = 0;
    float crossFadeStep = 0;

public :

    explicit Fader(const uint16_t effectArraySize);

    void changeEffect(Effect *effect) override;

    void render(DisplaySpec *displaySpec, CRGB *outputArray) override;
};

#endif //LED_SEGMENTS_FADER_H
