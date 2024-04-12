#include "pongmodifier.h"

const std::function<Modifier *(const Section &, const Mirror)> PongModifier::factory = [](
        const Section &section,
        const Mirror mirror
) -> Modifier * {
    return new PongModifier(section, mirror);
};

void PongModifier::setAlphaArray() {
    for (int i = section.start; i <= section.end; ++i) {
        alphaArray[i] = i == position ? 255 : 0;
    }

    isReversed ? --position : ++position;

    if (position == section.start || position == section.end) {
        isReversed = !isReversed;
    }
}