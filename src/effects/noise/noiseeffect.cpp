#include "noiseeeffect.h"
#include <functional>
#include <memory>

EffectFactory NoiseEffect::factory = [](
        const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::unique_ptr<Effect>(new NoiseEffect(effectContext));
};

void NoiseEffect::fillArray(
        CRGB *effectArray,
        const uint16_t effectArraySize,
        const uint16_t frameIndex
) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        uint8_t noiseScale = beatsin8(10, 10, 30);
        uint8_t noise = inoise8(i * noiseScale, millis() / noiseSpeed);
//        bool usePalette = effectContext.iteration % 2 == 0;
        bool usePalette = false;
        if (usePalette) {
            uint16_t index = map(noise, 50, 190, 0, effectArraySize); //increase contrast
            fill_palette(
                    effectArray,
                    effectArraySize,
                    index,
                    10,
                    effectContext.palette,
                    255,
                    LINEARBLEND
            );
        } else {
            uint16_t index = map(noise, 50, 190, 0, 255); //increase contrast
            effectArray[i] = CHSV(index, 255, 255); //noise rainbow
        }
    }
};

//TODO OctaveEffect: https://www.youtube.com/watch?v=7Dhh0IMSI4Q&ab_channel=ScottMarley