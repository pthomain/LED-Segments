#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include "engine/utils/utils.h"
#include "effectcontext.h"
#include "functional"

class Effect {
private:

    void shiftArrayValues(CRGB *array, uint16_t arraySize, uint16_t shift);

protected:
    uint8_t start = 0;
    int16_t speed = 0;//random8(5, 10);

public :
    const EffectContext effectContext;

    explicit Effect(EffectContext effectContext) : effectContext(std::move(effectContext)) {};

    void fillArray(
            CRGB *effectArray,
            uint16_t effectArraySize,
            uint16_t frameIndex
    );

    virtual void fillArrayInternal(
            CRGB *effectArray,
            uint16_t effectArraySize,
            uint16_t frameIndex
    ) = 0;

    virtual String name() const = 0;

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