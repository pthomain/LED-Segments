#ifndef LED_MATRIX_DLH_EFFECTCONFIG_H
#define LED_MATRIX_DLH_EFFECTCONFIG_H

#include <functional> // Include for std::function
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include "structure/cluster.h"

class EffectConfig {
public:
    const Cluster &scopeCluster;
    const std::function<Effect *(const Section &, const Mirror)> &effectFactory;
    const std::function<Effect *(const Section &, const Mirror)> *modifierFactory;
    const Cluster *pixelUnits;
    const Mirror mirror;

    EffectConfig(
        const Cluster &scopeCluster,
        const std::function<Effect *(const Section &, const Mirror)> &effectFactory,
        const std::function<Effect *(const Section &, const Mirror)> *modifierFactory,
        const Cluster *pixelUnits,
        const Mirror mirror
    ) : scopeCluster(scopeCluster),
        effectFactory(effectFactory),
        modifierFactory(modifierFactory),
        pixelUnits(pixelUnits),
        mirror(mirror) {
    };
};

#endif //LED_MATRIX_DLH_EFFECTCONFIG_H
