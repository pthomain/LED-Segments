#include <FastLED.h>
#include <effects.h>
#include <leds.h>

void setup() {
    Serial.begin(9600);
    delay(1500);
    initLeds();
}

void loop() {
//    CRGBPalette16 palette16;
//    int delayMs = 2000;
//
//    EVERY_N_MILLISECONDS(delayMs) {
//        palette16 = CRGBPalette16(
//                CHSV(random8(), 255, random8(128, 255)),
//                CHSV(random8(), 255, random8(128, 255)),
//                CHSV(random8(), 192, random8(128, 255)),
//                CHSV(random8(), 255, random8(128, 255))
//        );
//    };
//
//    EVERY_N_MILLISECONDS(delayMs)
//    {
//        applyEffect(
//                Effect(
//                        SCOPE_WHOLE,
//                        LTR,
//                        DIVISION_WHOLE,
//                        palette16
//                )
//        );
//    }
}