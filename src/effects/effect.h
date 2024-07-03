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

public :

    const Section section;
    const Mirror mirror;

    explicit Effect(
            const Section &section,
            const Mirror mirror
    ) : section(section), mirror(mirror) {
        arraySize = section.end - section.start + 1;
    };

    virtual ~Effect() = default;

    virtual void fillArray(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    static Effect *createEffect(const Section &section, const Mirror mirror) {
        return new T(section, mirror);
    }
};

#endif //EFFECTS_H