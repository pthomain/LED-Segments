#pragma once

#include <FastLED.h>
#include <utils.h>

uint16_t x = 0;
uint16_t time = 0;
uint8_t scale = 40;
uint8_t octave = 1;

uint8_t normaliseNoise(int8_t value) {
    return map(value, 30, 195, 0, 255);
}

//void showNoisePalette(CRGBPalette16 palette) {
//    uint8_t noiseData[NUM_LEDS];
//    memset8(noiseData, 0, NUM_LEDS);
//
//    EVERY_N_MILLISECONDS(100)
//    {
//        time = millis() / 10;
//        printNumber("Time", time);
//
//        x = beatsin8(11, 0, NUM_LEDS);
//        printNumber("X", time);
//
//        fill_raw_noise8(noiseData, NUM_LEDS, octave, x, scale, time);
//        Serial.println("Noise filled");
//
//        for (uint8_t i = 0; i < 10; i++) {
//            printNumber("Calculate pixel colour at pos", i);
//            auto colour = ColorFromPalette(
//                    palette,
//                    normaliseNoise(noiseData[i]),
//                    255
//            );
//            ledWhole[i] = colour;
//        }
//
//        Serial.println("Show noise");
//        FastLED.show();
//    }
//}