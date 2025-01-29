#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "engine/effect/effect.h"
#include "engine/displayspec.h"
#include "memory"

class Renderer {
protected:
    const DisplaySpec &displaySpec;
    const String name;

public:
    explicit Renderer(
        const DisplaySpec &displaySpec,
        const String &name
    ) : displaySpec(displaySpec), name(name) {
    }

    virtual bool hasEffect() = 0;

    virtual void changeEffect(std::unique_ptr<Effect> effect) = 0;

    virtual void render(CRGB *outputArray) = 0;

    virtual ~Renderer() = default;
};


#endif //LED_SEGMENTS_RENDERER_H
