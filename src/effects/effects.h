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

public:
    const Section section;
    const Mirror mirror;

    explicit Effect(
            const Section &section,
            const Mirror mirror
    ) : section(section), mirror(mirror) {
        arraySize = section.end - section.start + 1;
    };

    virtual ~Effect() = default; // Virtual destructor

    virtual void fillArray(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    std::unique_ptr<Effect> createEffect(const Section &section, const Mirror mirror) {
        return std::make_unique<T>(section, mirror);
    }
};

#endif //EFFECTS_H