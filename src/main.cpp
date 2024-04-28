#include <FastLED.h>
#include <utils.h>
#include <display.h>
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include <cluster.h>
#include <memory>
#include "effects/partyeffect.h"
#include "modifiers/pongmodifier.h"

#define CHANCE_OF_MODIFIER 5
#define CHANCE_OF_MIRROR 5

Display *display = nullptr;
auto scope = SCOPE_WHOLE;

auto effectPixelUnit = UNIT_LETTER;
auto modifierPixelUnit = UNIT_LETTER;

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

    EVERY_N_MILLISECONDS(80) {
        display->render();
    }
}

void changeEffect() {
//    std::pair<Scope, PixelUnit> variation = variations.at(random8(variations.size() - 1));
//     modifierScope = variation.first;
//     modifierPixelUnit = variation.second;
//
    bool useModifier = random8(CHANCE_OF_MODIFIER) == 0;
    if (!useModifier) display->clearModifier(scope);

    Mirror mirror;

//    if (random8(CHANCE_OF_MIRROR) == 0) {
//        mirror = mirrors.at(1 + random8(mirrors.size() - 2));
//    } else {
        mirror = MIRROR_NONE;
//    }

    //TODO fade effects https://github.com/atuline/FastLED-Demos/blob/master/aanimations/aanimations.ino
    display->applyEffectOrModifier(
            effectFactories.at(random8(effectFactories.size() - 1)),
            scope,
            effectPixelUnit,
            mirror,
            false
    );

    if (useModifier&&false) {
        display->applyEffectOrModifier(
                modifierFactories.at(random8(modifierFactories.size() - 1)),
                scope,
                modifierPixelUnit,
                mirror,
                true
        );
    }
}
