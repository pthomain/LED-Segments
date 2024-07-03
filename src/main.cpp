#include <FastLED.h>
#include "utils/utils.h"
#include "structure/display.h"
#include "effects/effect.h"
#include "modifiers/modifier.h"
#include "structure/cluster.h"
#include <memory>
#include "effects/partyeffect.h"
#include "modifiers/pongmodifier.h"
#include "effects/greeneffect.h"
#include "effects/redeffect.h"

#define CHANCE_OF_MODIFIER 0
#define CHANCE_OF_MIRROR 3
#define TRANSITION_DURATION_IN_MILLIS 1000
#define FPS 30
#define REFRESH_RATE_IN_MILLIS (1000 / FPS)
#define EFFECT_DURATION_IN_SECONDS 10

#define TRANSITION_DURATION_IN_MILLIS 1000
#define TRANSITION_DURATION_IN_FRAMES TRANSITION_DURATION_IN_MILLIS / REFRESH_RATE_IN_MILLIS

Display *display = nullptr;
auto scope = SCOPE_WHOLE;
auto pixelUnit = UNIT_LETTER;
Mirror mirror = MIRROR_NONE;
uint8_t variationIndex = 0;

std::function<Effect *(const Section &, const Mirror)> *modifierFactory = nullptr;

std::vector<std::function<Effect *(const Section &, Mirror)> > effectFactories;
std::vector<std::function<Effect *(const Section &, Mirror)> > modifierFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);

    effectFactories = {
            PartyEffect::factory,
            GreenEffect::factory,
            RedEffect::factory
    };

    modifierFactories = {
            PongModifier::factory
    };

    display = initDisplay(IS_PROD ? 50 : 10);
    changeEffect();
}

void loop() {
    EVERY_N_SECONDS(EFFECT_DURATION_IN_SECONDS) {
        changeEffect();
    }

    EVERY_N_MILLISECONDS(REFRESH_RATE_IN_MILLIS) {
        display->render();
    }
}

void changeEffect() {
    std::pair<Scope, PixelUnit> variation = variations.at(variationIndex);//random8(variations.size() - 1));
    scope = variation.first;
    pixelUnit = variation.second;

    variationIndex = (variationIndex + 1) % variations.size();

    mirror = MIRROR_NONE;
    if (random8(CHANCE_OF_MIRROR) == 1) {
        mirror = mirrors.at(1 + random8(mirrors.size() - 1)); // skip MIRROR_NONE
    }

    modifierFactory = nullptr;
    if (random8(CHANCE_OF_MODIFIER) == 1)
        modifierFactory = &(modifierFactories.at(random8(modifierFactories.size() - 1)));

    display->applyEffect(
            TRANSITION_DURATION_IN_FRAMES,
            effectFactories.at(random8(effectFactories.size() - 1)),
            modifierFactory,
            scope,
            pixelUnit,
            mirror
    );
}
