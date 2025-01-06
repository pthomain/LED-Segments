#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include <cstdint>
#include "effects/effect.h"
#include "displayspec/displayspec.h"
#include "render/renderer.h"

class Display {

private:

    const DisplaySpec &displaySpec;
    const std::unique_ptr<Renderer> renderer;
    const std::vector<EffectFactory> effectFactories;
    CRGB *outputArray;
    CRGB *effectArray;

public:

    explicit Display(
            const DisplaySpec &displaySpec,
            const std::vector<EffectFactory> effectFactories
    );

    void changeEffect();

    void render();

    ~Display() {
        delete[] outputArray;
        delete[] effectArray;
    }
};


#endif //LED_SEGMENTS_DISPLAY_H
