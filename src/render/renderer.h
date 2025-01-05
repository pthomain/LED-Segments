#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "effects/effect.h"
#include "displayspec/displayspec.h"
#include "memory"

class Renderer {

protected:

    std::shared_ptr<DisplaySpec> displaySpec;

public:

    explicit Renderer(std::shared_ptr<DisplaySpec> displaySpec) : displaySpec(displaySpec) {}

    virtual void changeEffect(std::unique_ptr<Effect> effect) = 0;

    virtual void render(CRGB *outputArray) = 0;

    virtual ~Renderer() = default;
};


#endif //LED_SEGMENTS_RENDERER_H
