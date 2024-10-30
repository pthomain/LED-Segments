#ifndef LED_SEGMENTS_EFFECTCONFIG_H
#define LED_SEGMENTS_EFFECTCONFIG_H

#include <functional> // Include for std::function
#include "effects/effect.h"
#include "config/effectcontext.h"
#include "modifiers/modifier.h"
#include "old_structure/cluster.h"

class EffectConfig {
public:
    const Cluster &scopeCluster;
    const std::function<Effect *(const EffectContext &effectContext)> &effectFactory;
    const std::function<Effect *(const EffectContext &effectContext)> *modifierFactory;
    const Cluster *pixelUnits;
    const Mirror mirror;

    EffectConfig(
        const Cluster &scopeCluster,
        const std::function<Effect *(const EffectContext &effectContext)> &effectFactory,
        const std::function<Effect *(const EffectContext &effectContext)> *modifierFactory,
        const Cluster *pixelUnits,
        const Mirror mirror
    ) : scopeCluster(scopeCluster),
        effectFactory(effectFactory),
        modifierFactory(modifierFactory),
        pixelUnits(pixelUnits),
        mirror(mirror) {
    };
};

#endif //LED_SEGMENTS_EFFECTCONFIG_H
