#ifndef LED_SEGMENTS_FADER_H
#define LED_SEGMENTS_FADER_H

#include "FastLED.h"
#include "render/canvas/canvas.h"

class Fader {
private:
    CRGB *outputArray;
    CRGB *firstCanvasOutputArray;
    CRGB *secondCanvasOutputArray;

    Canvas firstCanvas;
    Canvas secondCanvas;
    int totalLeds;

    bool isFirstCanvasRendering = false;
    uint16_t crossFadeMax = 0;
    uint16_t crossFadeStep = 0;

public :
    explicit Fader(CRGB *outputArray, int totalLeds) : outputArray(outputArray),
                                                       firstCanvas(totalLeds),
                                                       secondCanvas(totalLeds),
                                                       totalLeds(totalLeds) {
        Serial.println("Fader constructor");
        firstCanvasOutputArray = new CRGB[totalLeds];
        secondCanvasOutputArray = new CRGB[totalLeds];

        for (int i = 0; i < totalLeds; i++) {
            secondCanvasOutputArray[i] = CRGB::Black;
        }

        Serial.println("Fader constructor complete");
    };

    ~Fader() {
        delete[] firstCanvasOutputArray;
        delete[] secondCanvasOutputArray;
    }

    void applyEffects(
            const std::vector<std::pair<Effect *, std::vector<Pixel *>>>& effectsAndPixels,
            const uint16_t transitionDurationInFrames
    );

    void render();
};

#endif //LED_SEGMENTS_FADER_H
