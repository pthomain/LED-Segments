#ifndef LED_MATRIX_DLH_EFFECTCONFIG_H
#define LED_MATRIX_DLH_EFFECTCONFIG_H

#include <functional> // Include for std::function
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include "cluster.h"

class EffectConfig {

public:
    const boolean isModifier;
    const std::function<Effect *(const Section &, const Mirror)> &effectFactory;
    const Cluster *pixelUnits;
    const Mirror mirror;

    EffectConfig(
            const std::function<Effect *(const Section &, const Mirror)> &effectFactory,
            const Cluster *pixelUnits,
            const Mirror mirror,
            const boolean isModifier
    ) : isModifier(isModifier),
        effectFactory(effectFactory),
        pixelUnits(pixelUnits),
        mirror(mirror) {};
};

#endif //LED_MATRIX_DLH_EFFECTCONFIG_H
