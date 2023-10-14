#pragma once

#include <FastLED.h>
#include <leds.h>

uint16_t x = 0;
uint16_t time = 0;
uint8_t scale = 40;
uint8_t octave = 1;
uint8_t noiseData[NUM_LETTERS];

int8_t normaliseNoise(int8_t value) {
    return map(value, 30, 195, 0, 255);
}

void showNoisePalette(CRGBPalette16 palette) {
    EVERY_N_MILLISECONDS(100)
    {
        Serial.println("tick");

        time = millis() / 10;
        x = beatsin8(11, 0, NUM_LETTERS);

        memset(noiseData, 0, NUM_LETTERS);
        fill_raw_noise8(noiseData, NUM_LETTERS, octave, x, scale, time);
        Serial.println("tock");

        for (int8_t i = 0; i < NUM_LEDS; i++) {
            auto colour = ColorFromPalette(
                    palette,
                    normaliseNoise(noiseData[i]),
                    255
            );
            leds[i] = colour;
        }
        Serial.println("tack");

        FastLED.show();
    }
}