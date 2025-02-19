#pragma once

#ifndef LED_SEGMENTS_SEEDGENERATOR_H
#define LED_SEGMENTS_SEEDGENERATOR_H

#include <cstdint>

#include "seedgenerator.h"
#include "Arduino.h"
#include "utils.h"

void addEntropy() {
    uint8_t entropy = 0;
    for (int i = 0; i < 8; i++) {
        entropy = (entropy << 1) | (analogRead(i) & 1); //Use only LSB for more entropy
    }
    random16_add_entropy(entropy);
}

#endif //LED_SEGMENTS_SEEDGENERATOR_H