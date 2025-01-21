#ifndef LED_SEGMENTS_DLH_PHRASECONFIG_H
#define LED_SEGMENTS_DLH_PHRASECONFIG_H

#define NB_LEDS 191
#define NB_LETTERS 14
#define NB_WORDS 5

#include <cstdint>

constexpr static const uint16_t LETTERS[NB_LETTERS][2] = {
        {0,   12},
        {13,  25},
        {26,  41},
        {42,  56},
        {57,  74},

        {75,  82},

        {83,  93},
        {94,  104},
        {105, 117},
        {118, 135},

        {136, 143},

        {144, 161},
        {162, 177},
        {178, 190}
};

constexpr static const uint16_t WORDS[NB_WORDS][2] = {
        {0,   74},
        {75,  82},
        {83,  135},
        {136, 143},
        {144, 190}
};

#endif //LED_SEGMENTS_DLH_PHRASECONFIG_H
