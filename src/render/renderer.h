#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "effects/effect.h"
#include "displayspec/displayspec.h"

class Renderer {

protected:

    const uint16_t effectArraySize;

public:

    explicit Renderer(const uint16_t effectArraySize) :
            effectArraySize(effectArraySize) {}

    virtual void changeEffect(Effect* effect) = 0;

    virtual void render(DisplaySpec *displaySpec, CRGB *outputArray) = 0;
};


#endif //LED_SEGMENTS_RENDERER_H
