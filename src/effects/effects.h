#pragma once

#include <functional>
#include "FastLED.h"

class Effect {
private :
    const CRGBPalette16 palette;

public:
    Effect(
            const CRGBPalette16 &_palette
    ) : palette(_palette) {
    }

    virtual void fillArray(
            CRGB *targetArray,
            int start,
            int end
    ) = 0;
};