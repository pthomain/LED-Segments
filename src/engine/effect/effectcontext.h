#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "mirror.h"
#include "engine/utils/utils.h"
#include "interpolator.h"
#include "transition.h"

class EffectContext {
public:

    const boolean isDisplayCircular;
    const uint16_t layoutIndex;
    const uint16_t effectIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const InterpolatorType interpolatorType;
    const Transition transition;
    const Mirror transitionMirror;

    EffectContext(
            const boolean isDisplayCircular,
            const uint16_t layoutIndex,
            const uint16_t effectIndex,
            const CRGBPalette16 &palette,
            const Mirror mirror,
            const InterpolatorType interpolatorType,
            const Transition transition,
            const Mirror transitionMirror
    ) : isDisplayCircular(isDisplayCircular),
        layoutIndex(layoutIndex),
        effectIndex(effectIndex),
        palette(palette),
        mirror(mirror),
        interpolatorType(interpolatorType),
        transition(transition),
        transitionMirror(transitionMirror) {}
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
