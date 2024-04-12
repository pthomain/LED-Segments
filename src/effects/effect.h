#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include <functional>
#include "FastLED.h"
#include "section.h"
#include "utils.h"
#include "modifiers/modifier.h"
#include <memory>

class Effect {

protected:
    int arraySize;

public :
    const Modifier *modifier;

    explicit Effect(const Modifier *modifier) : modifier(modifier) {
        arraySize = modifier->section.end - modifier->section.start + 1;
    };

    virtual ~Effect() = default; // Virtual destructor

    virtual void fillArray(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    Effect *createEffect(const Modifier *modifier) {
        return new T(modifier);
    }
};

#endif //EFFECTS_H