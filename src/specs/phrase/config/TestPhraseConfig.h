#ifndef LED_SEGMENTS_TESTPHRASECONFIG_H
#define LED_SEGMENTS_TESTPHRASECONFIG_H

#define NB_LEDS 256
#define NB_LETTERS 10
#define NB_WORDS 3

#include <cstdint>

constexpr static const uint16_t LETTERS[NB_LETTERS][2] = {
        {0,   23},
        {24,  39},
        {40,  63},
        {64,  103},
        {104, 151},
        {152, 207},
        {208, 231},
        {232, 239},
        {240, 247},
        {248, 255}
};

constexpr static const uint16_t WORDS[NB_WORDS][2] = {
        {0,   103},
        {104, 231},
        {232, 255}
};

#endif //LED_SEGMENTS_TESTPHRASECONFIG_H
