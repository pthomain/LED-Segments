#include "swipeeffect.h"
#include "config/variation.h"
#include "modifiers/pongmodifier.h"

std::function<Effect *(const EffectContext &effectContext)> SwipeEffect::factory = [](
        const EffectContext &effectContext
) -> Effect * {
    return new SwipeEffect(effectContext);
};

void SwipeEffect::fillArrayInternal(CRGB *targetArray) {



};

Variation SwipeEffect::variation = Variation(
        {ALL_SCOPES_WEIGHTED},
        {ALL_MIRRORS_WEIGHTED},
        {}
);