#include <FastLED.h>
#include <utils.h>
#include <display.h>
#include <effects/effects.h>
#include <cluster.h>
#include <effects/pongeffect.h>
#include <effects/hueeffect.h>
#include <memory>

Scope scope;
Mirror mirror = MIRROR_EDGE;
uint8_t currentEffect = 0;
Display *display;

const std::vector<std::function<std::unique_ptr<Effect>(Section &, Mirror)>> effectFactories = {
        PongEffect::factory,
        HueEffect::factory
};

void changeEffect();

void setup() {
    Serial.begin(9600);
    delay(2000);

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();

    display = initDisplay(IS_PROD ? 50 : 10);
    changeEffect();
}

void loop() {
    EVERY_N_SECONDS(10) {
        changeEffect();
    }

    EVERY_N_SECONDS(30) {
        currentEffect = (currentEffect + 1) % effectFactories.size();
    }

    EVERY_N_MILLISECONDS(60) {
        display->render();
    }
}

void changeEffect() {
    std::pair<Scope, PixelUnit> variation = variations.at(random8(variations.size() - 1));
    scope = variation.first;
    auto pixelUnit = variation.second;
    mirror = mirrors.at(random8(mirrors.size() - 1));

    std::string output = "Scope: " + scopeToString(scope)
                         + ", PixelUnit: " + pixelUnitToString(pixelUnit)
                         + ", Mirror: " + mirrorToString(mirror);
    Serial.println(output.c_str());

    FastLED.clear();
    display->changeEffect(effectFactories.at(currentEffect), scope, pixelUnit, mirror);
}
