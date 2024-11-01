#include "noiseeeffect.h"
#include <functional>

std::function<Effect *(const EffectContext &effectContext)> NoiseEffect::factory = [](
        const EffectContext &effectContext
) -> Effect * {
    return new NoiseEffect(effectContext);
};

void NoiseEffect::fillArrayInternal(CRGB *targetArray) {
    for (uint16_t i = 0; i < arraySize; i++) {
        uint8_t noiseScale = beatsin8(10, 10, 30);
        uint8_t noise = inoise8(i * noiseScale, millis() / noiseSpeed);
        bool usePalette = effectContext.iteration() % 2 == 0;

        if (usePalette) {
            uint16_t index = map(noise, 50, 190, 0, arraySize); //increase contrast
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
            uint16_t index = map(noise, 50, 190, 0, 255); //increase contrast
            targetArray[i] = CHSV(index, 255, 255); //noise rainbow
        }
    }
};

//TODO OctaveEffect: https://www.youtube.com/watch?v=7Dhh0IMSI4Q&ab_channel=ScottMarley