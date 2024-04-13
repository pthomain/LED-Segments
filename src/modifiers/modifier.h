#pragma once

#ifndef MODIFIER_H
#define MODIFIER_H

#include <functional>
#include "FastLED.h"
#include "section.h"
#include "utils.h"
#include "effects/effect.h"
#include <memory>

class Modifier : public Effect {

protected:
    int arraySize;
    uint8_t *alphaArray;

    virtual void fillAlphaArray() = 0;

public:

    explicit Modifier(
            const Section &section,
            const Mirror mirror
    ) : Effect(section, mirror) {
        arraySize = Effect::arraySize;
        alphaArray = new uint8_t[arraySize];
    };

    ~Modifier() = default;
};

template<typename T>
class ModifierFactory {
public:
    Effect *createModifier(const Section &section, const Mirror mirror) {
        return new T(section, mirror);
    }
};

#endif //MODIFIER_H