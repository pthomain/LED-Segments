#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include "utils/utils.h"
#include "effectcontext.h"

class Effect {

protected:
    uint arraySize;
    int iteration = 0;
    CRGBPalette16 palette;
    EffectContext effectContext;

    uint8_t start = 0;
    uint8_t scale = 20;
    uint8_t speed = 0;

public :

    explicit Effect(EffectContext effectContext) : effectContext(effectContext) {
        arraySize = effectContext.arraySize();
        palette = PALETTES[effectContext.iteration() % PALETTES.size()];

        start = random8(arraySize);
        scale = 5 * PRIMES[random8(10)];
        speed = min(1, 3 * PRIMES[random8(10)]);

        if (true || effectContext.iteration() % 2 == 0) {
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
    static Effect *createEffect(const EffectContext &effectContext) {
        return new T(effectContext);
    }
};

#endif //EFFECTS_H