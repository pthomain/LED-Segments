#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "effects/effect.h"
#include "structure/segment.h"
#include "memory"

class Renderer {

protected:

    const uint16_t effectArraySize;

public:

    explicit Renderer(const uint16_t effectArraySize) :
            effectArraySize(effectArraySize) {}

    virtual void changeEffect(
            std::shared_ptr<Effect> effect,
            const std::vector<Segment *> &layout
    ) = 0;

    virtual void render(CRGB *outputArray) = 0;
};


#endif //LED_SEGMENTS_RENDERER_H
