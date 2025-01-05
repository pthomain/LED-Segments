#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include <cstdint>
#include "effects/effect.h"
#include "displayspec/displayspec.h"
#include "render/renderer.h"

class Display {

private:

    std::shared_ptr<DisplaySpec> displaySpec;
    const std::vector<EffectFactory> effectFactories;
    CRGB *outputArray;
    CRGB *effectArray;
    Renderer *renderer;

public:

    explicit Display(
            std::shared_ptr<DisplaySpec> displaySpec,
            std::vector<EffectFactory> effectFactories
    );

    void changeEffect();

    void render();

    ~Display() {
        delete[] outputArray;
        delete[] effectArray;
        delete renderer;
    }
};


#endif //LED_SEGMENTS_DISPLAY_H
