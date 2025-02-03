#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "mirror.h"
#include "engine/utils/utils.h"
#include "interpolator.h"

enum EffectCycle {
    WRAP, //starts back from 0 after reaching 255
    BOUNCE //changes direction after reaching 255
};

class EffectContext {
public:

    const EffectCycle cycle;
    const uint16_t layoutIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const InterpolatorType interpolatorType;

    EffectContext(
            const EffectCycle cycle,
            const uint16_t layoutIndex,
            const CRGBPalette16 &palette,
            const Mirror mirror,
            const InterpolatorType interpolatorType
    ) : cycle(cycle),
        layoutIndex(layoutIndex),
        palette(palette),
        mirror(mirror),
        interpolatorType(interpolatorType) {}
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
