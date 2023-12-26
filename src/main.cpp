#include <FastLED.h>
#include "effects/hueeffect.h"
#include <display.h>

Scope scope = SCOPE_WHOLE;
PixelUnit pixelUnit = UNIT_PIXEL;
Display* display;
const HueEffect& effect = HueEffect(RainbowColors_p);
int currentScope = 0;
int currentUnit = 0;

void setup() {
    Serial.begin(9600);
    delay(2000);

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();

    display = initDisplay();
}

void loop() {
    // EVERY_N_SECONDS(5) {
    //     if (currentScope % 4 == 0) {
    //         scope = SCOPE_WHOLE;
    //
    //         if (currentUnit % 4 == 0) {
    //             pixelUnit = UNIT_WORD;
    //             Serial.println("SCOPE_WHOLE UNIT_WORD");
    //         } else if (currentUnit % 4 == 1) {
    //             pixelUnit = UNIT_LETTER;
    //             Serial.println("SCOPE_WHOLE UNIT_LETTER");
    //         } else if (currentUnit % 4 == 2) {
    //             pixelUnit = UNIT_COLUMN;
    //             Serial.println("SCOPE_WHOLE UNIT_COLUMN");
    //         } else {
    //             pixelUnit = UNIT_PIXEL;
    //             Serial.println("SCOPE_WHOLE UNIT_PIXEL");
    //             currentScope++;
    //         }
    //     } else if (currentScope % 4 == 1) {
    //         scope = SCOPE_WORD;
    //
    //         if (currentUnit % 4 == 0) {
    //             pixelUnit = UNIT_LETTER;
    //             Serial.println("SCOPE_WORD UNIT_LETTER");
    //         } else if (currentUnit % 4 == 1) {
    //             pixelUnit = UNIT_COLUMN;
    //             Serial.println("SCOPE_WORD UNIT_COLUMN");
    //         } else {
    //             pixelUnit = UNIT_PIXEL;
    //             Serial.println("SCOPE_WORD UNIT_PIXEL");
    //             currentScope++;
    //         }
    //     } else if (currentScope % 4 == 2) {
    //         scope = SCOPE_LETTER;
    //
    //         if (currentUnit % 4 == 0) {
    //             pixelUnit = UNIT_COLUMN;
    //             Serial.println("SCOPE_LETTER UNIT_COLUMN");
    //         } else {
    //             pixelUnit = UNIT_PIXEL;
    //             Serial.println("SCOPE_LETTER UNIT_PIXEL");
    //             currentScope++;
    //         }
    //     } else {
    //         scope = SCOPE_COLUMN;
    //         pixelUnit = UNIT_PIXEL;
    //         Serial.println("SCOPE_COLUMN UNIT_PIXEL");
    //         currentScope++;
    //     }
    //
    //     currentUnit++;
    // }


    EVERY_N_MILLIS(1000) {
        display->render(
            (Effect &) effect,
            SCOPE_WHOLE,
            UNIT_PIXEL
        );
    }
}
