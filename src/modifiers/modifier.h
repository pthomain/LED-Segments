#pragma once

#ifndef MODIFIER_H
#define MODIFIER_H

#include <functional>
#include "FastLED.h"
#include "structure/section.h"
#include "utils/utils.h"
#include "effects/effect.h"
#include "config/effectcontext.h"
#include <memory>

class Modifier : public Effect {

protected:
    uint16_t arraySize;
    uint8_t *alphaArray;

    virtual void fillAlphaArray() = 0;

public:

    explicit Modifier(const EffectContext &effectContext) : Effect(effectContext) {
        arraySize = Effect::arraySize;
        alphaArray = new uint8_t[arraySize];
    };

    ~Modifier() override {
        delete[] alphaArray;
    }

    void fillArrayInternal(CRGB *targetArray) override {
        fillAlphaArray();
        for (int i = 0; i < arraySize; ++i) {
            targetArray[i].nscale8(alphaArray[i]);
        }
    }
};

template<typename T>
class ModifierFactory {
public:
    static Effect *createModifier(const EffectContext &effectContext) {
        return new T(effectContext);
    }
};

#endif //MODIFIER_H