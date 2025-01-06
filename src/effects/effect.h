#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include "utils/utils.h"
#include "effectcontext.h"
#include "functional"
#include "memory"

class Effect {

protected:
    int iteration = 0;
    uint8_t start = 0;
    uint8_t scale = 20;
    uint8_t speed = 0;

public :
    const EffectContext effectContext;

    explicit Effect(const EffectContext &effectContext) : effectContext(effectContext) {
        scale = 5 * PRIMES[random8(10)];
        speed = min(1, 3 * PRIMES[random8(10)]);
    };

    virtual void fillArray(CRGB *effectArray, uint16_t effectArraySize) = 0;

    virtual ~Effect() = default;

    template<typename T>
    class Factory {
    public:
        static std::unique_ptr<Effect> createEffect(const EffectContext &effectContext) {
            return std::unique_ptr<Effect>(new T(effectContext));
        }
    };

};

using EffectFactory = std::function<std::unique_ptr<Effect>(const EffectContext &effectContext)>;

#endif //EFFECTS_H