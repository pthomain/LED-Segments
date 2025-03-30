#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include "engine/utils/utils.h"
#include "effectcontext.h"
#include "functional"

const uint8_t MIN_CYCLE_SPEED = 5;
const uint8_t MAX_CYCLE_SPEED = 15;

class Effect {
private:

protected:
    uint16_t linearCycleStep = 0;
    uint16_t circularCycleStep = 0;
    uint8_t cycleSpeed = 5;//random8(MIN_CYCLE_SPEED, MAX_CYCLE_SPEED);
    boolean isCycleReversed = false;

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