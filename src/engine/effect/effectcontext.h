#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "mirror.h"
#include "engine/utils/utils.h"
#include "interpolator.h"

class EffectContext {
public:

    const boolean isDisplayCircular;
    const uint16_t layoutIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const InterpolatorType interpolatorType;

    EffectContext(
            const boolean isDisplayCircular,
            const uint16_t layoutIndex,
            const CRGBPalette16 &palette,
            const Mirror mirror,
            const InterpolatorType interpolatorType
    ) : isDisplayCircular(isDisplayCircular),
        layoutIndex(layoutIndex),
        palette(palette),
        mirror(mirror),
        interpolatorType(interpolatorType) {}
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
