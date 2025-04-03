#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include <utility>

#include "engine/effect/effect.h"
#include "engine/displayspec/displayspec.h"
#include "engine/effect/transition.h"

class Renderer {
protected:
    const DisplaySpec &displaySpec;
    const String name;

public:
    explicit Renderer(
            const DisplaySpec &displaySpec,
            String name
    ) : displaySpec(displaySpec), name(std::move(name)) {
    }

    virtual void changeEffect(std::shared_ptr<Effect> effect) = 0;

    virtual void render(CRGB *outputArray) = 0;

    virtual std::shared_ptr<Effect> getEffect() = 0;

    virtual ~Renderer() = default;
};


#endif //LED_SEGMENTS_RENDERER_H
