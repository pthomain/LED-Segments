#ifndef LED_MATRIX_DLH_FADER_H
#define LED_MATRIX_DLH_FADER_H

#include "FastLED.h"
#include "render/canvas.h"

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
        firstCanvasOutputArray = new CRGB[totalLeds];
        secondCanvasOutputArray = new CRGB[totalLeds];

        for (int i = 0; i < totalLeds; i++) {
            secondCanvasOutputArray[i] = CRGB::Black;
        }
    };

    ~Fader() {
        delete[] firstCanvasOutputArray;
        delete[] secondCanvasOutputArray;
    }

    void applyConfig(
            EffectConfig *effectConfig,
            const uint16_t transitionDurationInFrames
    );

    void render();
};

#endif //LED_MATRIX_DLH_FADER_H
