#include <FastLED.h>
#include "effects/hueeffect.h"
#include <leds.h>

Scope scope;
Display *display;
const HueEffect &effect = HueEffect(RainbowColors_p);
int8_t currentScope = 0;

void updateEffectIfChanged(Effect &effect);

void setup() {
    Serial.begin(9600);
    delay(1000);
    display = initDisplay();

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();
    FastLED.show();
}


void loop() {
    EVERY_N_SECONDS(5) {
        currentScope++;

        if (currentScope % 3 == 0) {
            scope = SCOPE_LETTER;
        } else if (currentScope % 3 == 1) {
            scope = SCOPE_WORD;
        } else {
            scope = SCOPE_WHOLE;
        }

        printNumber("Scope", scope);
    };

    EVERY_N_MILLIS(32) {
        display->render((Effect &) effect, scope);
    }
}