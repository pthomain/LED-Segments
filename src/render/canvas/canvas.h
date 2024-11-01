#ifndef LED_SEGMENTS_CANVAS_H
#define LED_SEGMENTS_CANVAS_H

#include "FastLED.h"
#include "utils/utils.h"
#include <utility>
#include <vector>
#include "effects/effect.h"
#include "../../structure/pixel/Pixel.h"

class Canvas {

    const uint16_t totalLeds = 0;
    CRGB *effectBufferArray = nullptr;
    CRGB *allEffectsArray = nullptr;

public :
    explicit Canvas(const uint16_t totalLeds) : totalLeds(totalLeds) {
        effectBufferArray = new CRGB[totalLeds];
        allEffectsArray = new CRGB[totalLeds];
    };

    void applyEffects(
            const std::vector<std::pair<Effect *, std::vector<Pixel *>>>& effectsAndPixels
    );

    void render(CRGB *outputArray);

    ~Canvas() {
        delete[] effectBufferArray;
    };
};

#endif //LED_SEGMENTS_CANVAS_H

