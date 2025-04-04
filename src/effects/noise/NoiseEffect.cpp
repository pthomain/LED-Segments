#include "NoiseEffect.h"
#include <functional>

EffectFactory NoiseEffect::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<NoiseEffect>(effectContext);
};

void NoiseEffect::fillArrayInternal(
    CRGB *effectArray,
    const uint16_t effectArraySize,
    const uint16_t frameIndex
) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        const uint8_t noiseScale = beatsin8(10, 10, 30);
        const uint8_t noise = inoise8(i * noiseScale, millis() / noiseSpeed);

        //        bool usePalette = effectContext.iteration % 2 == 0;
        bool usePalette = false;
        if (usePalette) {
            fill_palette(
                effectArray,
                effectArraySize,
                increaseContrast(noise),
                10,
                effectContext.palette,
                255,
                LINEARBLEND
            );
        } else {
            effectArray[i] = CHSV(increaseContrast(noise), 255, 255); //noise rainbow
        }
    }
};

//TODO OctaveEffect: https://www.youtube.com/watch?v=7Dhh0IMSI4Q&ab_channel=ScottMarley
