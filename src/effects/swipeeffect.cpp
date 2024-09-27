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
        {std::make_pair(SCOPE_WHOLE, UNIT_LETTER)},
        {MIRROR_NONE},
        {}
);