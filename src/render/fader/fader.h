#ifndef LED_SEGMENTS_FADER_H
#define LED_SEGMENTS_FADER_H

#include "FastLED.h"
#include "BitArray.h"
#include "config.h"
#include "effects/effect.h"
#include "structure/segment.h"
#include "render/renderer.h"

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

    void changeEffect(
            std::shared_ptr<Effect> effect,
            const std::vector<Segment *> &layout
    ) override;

    void render(CRGB *outputArray) override;
};

#endif //LED_SEGMENTS_FADER_H
