#pragma once

#ifndef LED_SEGMENTS_SEEDGENERATOR_H
#define LED_SEGMENTS_SEEDGENERATOR_H

#include <cstdint>
#include "Arduino.h"

//Provide any free pin to use to gather electrical noise to build entropy for the PRNG.
//Be careful not to include any pin already used in your circuit.
void addEntropy(const uint8_t *freePins, const uint8_t nbFreePins) {
    uint8_t entropy = 0;
    for (uint8_t i = 0; i < nbFreePins; i++) {
        entropy = (entropy << 1) | (analogRead(i) & 1); //Use only LSB for more entropy
    }
    random16_add_entropy(entropy);
}

#endif //LED_SEGMENTS_SEEDGENERATOR_H
