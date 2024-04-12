#include <FastLED.h>
#include <utils.h>
#include <display.h>
#include "effects/effect.h"
#include <cluster.h>
#include <memory>
#include "effects/partyeffect.h"
#include "effects/testeffect.h"
#include "modifiers/pongmodifier.h"

Mirror mirror = MIRROR_NONE;
uint8_t currentEffect = 0;
uint8_t currentModifier = 0;
Display *display = nullptr;

std::vector<std::function<Modifier *(const Section &, Mirror)>> modifierFactories;
std::vector<std::function<Effect *(const Modifier *)>> effectFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);
    delay(2000);

    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();

    modifierFactories = {
            PongModifier::factory
    };

    effectFactories = {
            PartyEffect::factory
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

    currentModifier = random8(mirrors.size() - 1);

    display->changeEffect(
            effectFactories.at(currentEffect),
            modifierFactories.at(currentModifier),
            scope,
            pixelUnit,
            mirror
    );
}
