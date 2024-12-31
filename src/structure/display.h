#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include <cstdint>
#include "vector"
#include <functional>
#include "effects/effect.h"
#include "segment.h"
#include "render/fader/fader.h"
#include "render/simplerenderer/simplerenderer.h"

class Display {

    CRGB *outputArray;
    CRGB *effectArray;

    std::vector<std::vector<Segment *>> layouts;
    Renderer *renderer;

public:

    explicit Display(
            uint16_t totalLeds,
            std::vector<std::vector<Segment *>> layouts
    );

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
