#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "render/renderer.h"

class SimpleRenderer : public Renderer {

private:
    CRGB *effectArray;
    Effect *currentEffect;

public :

    explicit SimpleRenderer(const uint16_t effectArraySize);

    void changeEffect(Effect *effect) override;

    void render(DisplaySpec *displaySpec, CRGB *outputArray) override;

};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
