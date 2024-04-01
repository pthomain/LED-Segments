#pragma once

#ifndef EFFECTS_H
#define EFFECTS_H

#include <functional>
#include "FastLED.h"
#include "section.h"
#include "utils.h"
#include <memory>

class Effect {

public:
    const Section section;

    explicit Effect(const Section &section) : section(section) {};

    virtual ~Effect() = default; // Virtual destructor

    virtual void fillArray(CRGB *targetArray) = 0;
};

template<typename T>
class EffectFactory {
public:
    std::unique_ptr<Effect> createEffect(const Section &section) {
        return std::make_unique<T>(section);
    }
};

#endif //EFFECTS_H