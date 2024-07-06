#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include <functional>
#include "FastLED.h"
#include "structure/section.h"
#include "utils/utils.h"
#include <memory>

class Effect {

protected:
    int arraySize;
    int iteration = 0;
    CRGBPalette16 palette;

    uint8_t start = 0;
    uint8_t scale = 20;
    uint8_t speed = 0;

    const uint8_t seed;

public :

    const Section section;
    const Mirror mirror;

    explicit Effect(
            const Section &section,
            const Mirror mirror,
            const uint8_t seed
    ) : section(section), mirror(mirror), seed(seed) {
        arraySize = section.end - section.start + 1;
        palette = PALETTES[seed % PALETTES.size()];

        start = random8(arraySize);
        scale = 5 * PRIMES[random8(10)];
        speed = min(1, 3 * PRIMES[random8(10)]);

        if (seed % 2 == 0) {
            iteration += speed;
        } else {
            iteration -= speed;
        }
    };

    virtual ~Effect() = default;

    void fillArray(CRGB *targetArray) {
        fillArrayInternal(targetArray);
        iteration++;
    }

    virtual void fillArrayInternal(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    static Effect *createEffect(const Section &section, const Mirror mirror, uint8_t seed) {
        return new T(section, mirror, seed);
    }
};

#endif //EFFECTS_H