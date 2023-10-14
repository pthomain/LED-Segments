#pragma once

#include <palettes.h>
#include "leds.h"

typedef enum {
    SCOPE_LETTER = 0,
    SCOPE_WORD = 1,
    SCOPE_WHOLE = 2
} Scope;

typedef enum {
    LTR = 0,
    RTL = 1,
} Direction;

typedef enum {
    DIVISION_CENTRE = 0,
    DIVISION_WHOLE = 1,
} Division;

struct Effect {
    Scope scope;
    Direction direction;
    Division division;
    CRGBPalette16 palette;

    Effect(
            Scope scope,
            Direction direction,
            Division division,
            CRGBPalette16 palette
    ) {
        this->scope = scope;
        this->direction = direction;
        this->division = division;
        this->palette = palette;
    }
};

void applyEffect(Effect effect) {
//    showNoisePalette(effect.palette);
};