#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "effects/mirror/mirror.h"

class EffectContext {

public:

    const uint16_t layoutIndex;
    const Mirror mirror;
    const uint16_t effectIndex;
    const CRGBPalette16 palette;

    EffectContext(
            const uint16_t layoutIndex,
            const Mirror mirror,
            const uint16_t effectIndex
    ) : layoutIndex(layoutIndex),
        mirror(mirror),
        effectIndex(effectIndex),
        palette(PALETTES[random8(PALETTES.size())]) {}

};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
