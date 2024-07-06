#include "effects/effect.h"
#include "modifier.h"
#include "stackmodifier.h"

std::function<Effect *(const Section &, const Mirror, uint8_t)> StackModifier::factory = [](
        const Section &section,
        const Mirror mirror,
        uint8_t seed
) -> Effect * {
    return new StackModifier(section, mirror, seed);
};

void StackModifier::fillAlphaArray() {
};
