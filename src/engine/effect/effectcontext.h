#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include "FastLED.h"
#include "mirror.h"
#include "engine/utils/utils.h"
#include "interpolator.h"

class EffectContext {
// Scenes handle mirrors, segment skipping, decorates the rendering, contain a single effect, contains an interpolator

public:

    const uint16_t layoutIndex;
    const CRGBPalette16 palette;
    const Mirror mirror;
    const InterpolatorType interpolatorType;

    EffectContext(
            const uint16_t layoutIndex,
            const CRGBPalette16 &palette,
            const Mirror mirror,
            const InterpolatorType interpolatorType
    ) : layoutIndex(layoutIndex),
        palette(palette),
        mirror(mirror),
        interpolatorType(interpolatorType) {}
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
