#include <FastLED.h>
#include <utils.h>
#include <display.h>
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include <cluster.h>
#include <memory>
#include "effects/partyeffect.h"
#include "modifiers/pongmodifier.h"

Display *display = nullptr;
int currentEffect = 0;

std::vector<std::function<Effect *(const Section &, Mirror)>> effectFactories;
std::vector<std::function<Effect *(const Section &, Mirror)>> modifierFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);
    delay(2000);

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();

    effectFactories = {
            PartyEffect::factory
    };

    modifierFactories = {
            PongModifier::factory
    };

    display = initDisplay(IS_PROD ? 50 : 10);
    changeEffect();
}

void loop() {
    EVERY_N_SECONDS(10) {
        changeEffect();
    }

    EVERY_N_MILLISECONDS(60) {
        display->render();
    }
}

void changeEffect() {
    currentEffect = (currentEffect + 1) % effectFactories.size();

    std::pair<Scope, PixelUnit> variation = variations.at(random8(variations.size() - 1));
    auto scope = variation.first;
    auto pixelUnit = variation.second;

    Mirror mirror = mirrors.at(random8(mirrors.size() - 1));
    int currentModifier = random8(modifierFactories.size() - 1);

    display->applyEffect(
            effectFactories.at(currentEffect),
            scope,
            pixelUnit,
            mirror,
            false
    );

    display->applyEffect(
            modifierFactories.at(currentEffect),
            scope,
            pixelUnit,
            mirror,
            true
    );
}
