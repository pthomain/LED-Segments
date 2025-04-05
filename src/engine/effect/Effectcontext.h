#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "utils/Utils.h"
#include "Transition.h"

class EffectContext {
public:
    const boolean isDisplayCircular;
    const uint16_t layoutIndex;
    const uint16_t effectIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const Transition transition;
    const uint8_t transitionLayoutIndex;
    const Mirror transitionMirror;

    EffectContext(
        const boolean isDisplayCircular,
        const uint16_t layoutIndex,
        const uint16_t effectIndex,
        const CRGBPalette16 &palette,
        const Mirror mirror,
        const Transition transition,
        const uint8_t transitionLayoutIndex,
        const Mirror transitionMirror
    ) : isDisplayCircular(isDisplayCircular),
        layoutIndex(layoutIndex),
        effectIndex(effectIndex),
        palette(palette),
        mirror(mirror),
        transition(transition),
        transitionLayoutIndex(transitionLayoutIndex),
        transitionMirror(transitionMirror) {
    }
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
