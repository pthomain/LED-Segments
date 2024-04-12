#pragma once

#ifndef MODIFIER_H
#define MODIFIER_H

#include <functional>
#include "FastLED.h"
#include "section.h"
#include "utils.h"
#include <memory>

class Modifier {

protected:
    int arraySize;
    uint8_t *alphaArray;

    virtual void setAlphaArray() = 0;

public:
    const Section section;
    const Mirror mirror;

    explicit Modifier(
            const Section &section,
            const Mirror mirror
    ) : section(section), mirror(mirror) {
        arraySize = section.end - section.start + 1;
        alphaArray = new uint8_t[arraySize];
    };

    virtual ~Modifier() = default; // Virtual destructor

    void applyAlpha(CRGB *targetArray) {
        for (int i = 0; i < arraySize; ++i) {
            targetArray[i] = targetArray[i].nscale8(alphaArray[i]);
        }
    }
};

template<typename T>
class ModifierFactory {
public:
    Modifier *createModifier(const Section &section, const Mirror mirror) {
        return new T(section, mirror);
    }
};

#endif //MODIFIER_H