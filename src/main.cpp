#include <FastLED.h>
#include "config.h"
#include "utils/utils.h"
#include "effects/effect.h"
#include "effects/party/partyeffect.h"
#include "utils/seedgenerator.h"
#include "displays/builder/DisplayBuilder.h"
#include "displays/display/display.h"

Display *display = nullptr;
std::vector<std::function<Effect *(const EffectContext &effectContext)>> effectFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);
    initSeed();

    set_max_power_in_volts_and_milliamps(5, 1500);
    FastLED.setBrightness(30);

    effectFactories = {
            PartyEffect::factory,
//            NoiseEffect::factory
    };

    display = DisplayBuilder().build(TEST);
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
    //TODO add stack modifier, each pixel stacks on the previous one + reverse
    //TODO add swipe effect, one colour slides over the previous one
    //TODO add word dwell modifier, dwell on each word for a while
    //TODO add chase with trail modifier, like ping pong but with a trail
    //TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
    addEntropy();
    display->changeEffects(
            TRANSITION_DURATION_IN_FRAMES,
            effectFactories
    );
}
