#include <FastLED.h>
#include <leds.h>
#include "MSGEQ7.h"

#define RESET 8
#define AUDIO_OUT 7
#define STROBE 4
#define MSGEQ7_INTERVAL ReadsPerSecond(10)
#define MSGEQ7_SMOOTH 0 // Range: 0-255

// Single MSGEQ7 IC, Smoothing enabled (75%)
//CMSGEQ7<MSGEQ7_SMOOTH, RESET, STROBE, AUDIO_OUT> MSGEQ7;

void setup() {
    Serial.begin(9600);
    delay(1000);

    initLeds();
//  MSGEQ7.begin();
}

void loop() {
//    bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

    // Led output
//    if (newReading) {
//        Serial.println(MSGEQ7.get(0)/5);
//    }
    showRainbowSwipe();
    // showNoisePalette();
}