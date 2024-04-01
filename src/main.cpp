#include <FastLED.h>
#include <utils.h>
#include <display.h>
#include <effects/effects.h>
#include <cluster.h>
#include <effects/pongeffect.h>
#include <effects/hueeffect.h>
#include <memory>

Scope scope;
PixelUnit pixelUnit;
int config = 5;
Display *display;

void updateEffect();

void setup() {
    Serial.begin(9600);
    delay(2000);

//    Serial.println("Starting...");
    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();
//    Serial.println("cleared...");

    display = initDisplay(IS_PROD ? 10 : 30);
//    Serial.println("inited...");
    updateEffect();
}

void loop() {
    EVERY_N_SECONDS(5) {
        config = (config + 1) % variations.size();
        updateEffect();
    }

    EVERY_N_MILLIS(50) {
        display->render(
                scope,
                pixelUnit
        );
    }
}

void updateEffect() {
    scope = variations.at(config).first;
    pixelUnit = variations.at(config).second;
    const auto &string = "apply effect: " + scopeToString(scope) + " " + pixelUnitToString(pixelUnit);
    Serial.println(string.c_str());
    FastLED.clear();
    display->applyEffect(PongEffect::factory, scope, pixelUnit);
}
