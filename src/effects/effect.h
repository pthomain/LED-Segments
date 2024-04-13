#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include <functional>
#include "FastLED.h"
#include "section.h"
#include "utils.h"
#include <memory>

class Effect {

protected:
    int arraySize;

public :

    const Section section;
    const Mirror mirror;

    explicit Effect(
            const Section &section,
            const Mirror mirror
    ) : section(section), mirror(mirror) {
        arraySize = section.end - section.start + 1;
    };

    ~Effect() = default;

    virtual void fillArray(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    Effect *createEffect(const Section &section, const Mirror mirror) {
        return new T(section, mirror);
    }
};

#endif //EFFECTS_H