#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "effects/mirror/mirror.h"

class EffectContext {

public:
    const CRGBPalette16 palette;
    const Mirror mirror;
    const uint16_t position;
    const uint16_t iteration;

    EffectContext(
            const Mirror mirror,
            const uint16_t position,
            const uint16_t iteration
    ) : mirror(mirror),
        position(position),
        iteration(iteration),
        palette(PALETTES[iteration % PALETTES.size()]) {
    }

};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
