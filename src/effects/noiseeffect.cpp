#include "noiseeeffect.h"
#include "config/variation.h"
#include "modifiers/pongmodifier.h"

std::function<Effect *(const Section &, const Mirror, uint8_t seed)> NoiseEffect::factory = [](
        const Section &section,
        const Mirror mirror,
        const uint8_t seed
) -> Effect * {
    return new NoiseEffect(section, mirror, seed);
};

void NoiseEffect::fillArrayInternal(CRGB *targetArray) {
    uint8_t brightnessScale = 50;

    for (int i = 0; i < arraySize; i++) {
        uint8_t noiseScale = beatsin8(10, 10, 30);
        uint8_t noise = inoise8(i * noiseScale, millis() / noiseSpeed);
        bool usePalette = false;//seed % 2 == 0;

        if (usePalette) {
            int index = map(noise, 50, 190, 0, arraySize); //increase contrast
            fill_palette(
                    targetArray,
                    arraySize,
                    index,
                    10,
                    palette,
                    255,
                    LINEARBLEND
            );
        } else {
            int index = map(noise, 50, 190, 0, 255); //increase contrast
            targetArray[i] = CHSV(index, 255, 255); //noise rainbow
        }
    }
};

Variation NoiseEffect::variation = Variation(
        {
                std::make_pair(SCOPE_WHOLE, UNIT_LETTER),
                std::make_pair(SCOPE_LETTER, UNIT_PIXEL),
        },
        ALL_MIRRORS,
        {
                nullptr,
//                &PongModifier::factory
        }
);
