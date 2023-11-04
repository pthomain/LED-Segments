#pragma once

#include "FastLED.h"

class Effect {
private :
    const CRGBPalette16 palette;

public:
    Effect(
            const CRGBPalette16 &_palette
    ) : palette(_palette) {
    }

    virtual void nextFrame(CRGB *targetArray, int arraySize) = 0;
};