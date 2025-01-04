#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include <cstdint>
#include "effects/effect.h"
#include "displayspec/displayspec.h"
#include "render/renderer.h"

class Display {

private:

    DisplaySpec *displaySpec;
    CRGB *outputArray;
    CRGB *effectArray;
    Renderer *renderer;

public:

    explicit Display(DisplaySpec *displaySpec);

    void changeEffect(
            const std::vector<EffectFactory> &effectFactories
    );

    void render();

    ~Display() {
        delete[] outputArray;
        delete[] effectArray;
    }
};


#endif //LED_SEGMENTS_DISPLAY_H
