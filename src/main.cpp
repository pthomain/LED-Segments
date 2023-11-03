#include <FastLED.h>
#include <effects.h>
#include <leds.h>

#define FRAMES_PER_SECOND 30

uint8_t lastEffect = -1;
uint8_t effect = 0;
Scope scope;
Display *display;

void updateEffectIfChanged(const Effect &effect);

void setup() {
    Serial.begin(9600);
    delay(1000);
    display = initDisplay();

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();
    FastLED.show();
}

void loop() {
//    FastLED.delay(1000/FRAM
//    ES_PER_SECOND);


    updateEffectIfChanged(Effect(RainbowColors_p));
//    EVERY_N_MILLIS(5000) {
//        effect += 1;
//    }
}

void updateEffectIfChanged(const Effect &effect2) {
//    if (lastEffect != effect) {

    EVERY_N_SECONDS(5) { effect++; };

    if (effect % 3 == 0) {
        scope = SCOPE_WHOLE;
    } else if (effect % 3 == 1) {
        scope = SCOPE_WORD;
    } else {
        scope = SCOPE_LETTER;
    }

    display->render(
            effect2,scope
    );
//    lastEffect = effect;
//    }
}