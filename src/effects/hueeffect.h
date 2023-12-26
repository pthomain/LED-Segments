#pragma once

#include "effects.h"
#include "utils.h"

class HueEffect : public Effect {
private :
    const CRGBPalette16 palette;

    // List of patterns to cycle through.  Each is defined as a separate function below.
//    typedef void (*SimplePatternList[])(CRGB *targetArray, int arraySize);

    int gCurrentPatternNumber = 0; // Index number of which pattern is current
    int gHue = 0; // rotating "base color" used by many of the patterns

    int position = 0;

    void nextPattern();

    void rainbowWithGlitter(CRGB *targetArray, int arraySize);

    void addGlitter(CRGB *targetArray, int arraySize, fract8 chanceOfGlitter);

    void confetti(CRGB *targetArray, int arraySize);

    void sinelon(CRGB *targetArray, int arraySize);

    void bpm(CRGB *targetArray, int arraySize);

    void juggle(CRGB *targetArray, int arraySize);

    void rainbow(
            CRGB *targetArray,
            int start,
            int end
    );

    void rgbGradient(
            CRGB *targetArray,
            int start,
            int end
    );

    void rgb(
            CRGB *targetArray,
            int start,
            int end
    );

    void rgbw(
            CRGB *targetArray,
            int start,
            int end
    );

    void pong(
            CRGB *targetArray,
            int arraySize
    );

//    SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};

public:
    HueEffect(const CRGBPalette16 &palette) : Effect(palette) {}

    void fillArray(
            CRGB *targetArray,
            int start,
            int end
    ) override;
};
