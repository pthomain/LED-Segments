#include <FastLED.h>
#include "config.h"
#include "effects/effect.h"
#include "effects/party/partyeffect.h"
#include "utils/seedgenerator.h"
#include "structure/display.h"
#include "displayspec/specs/TestSpec.h"
#include "effects/noise/noiseeeffect.h"

DisplaySpec *spec = (DisplaySpec *) new TestSpec();
Display display = Display(spec);
std::vector<EffectFactory> effectFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);
    addEntropy();
    delay(2000);

    effectFactories = {
            PartyEffect::factory,
            NoiseEffect::factory

    };

    changeEffect();
}

void loop() {
    EVERY_N_SECONDS(EFFECT_DURATION_IN_SECONDS) {
        changeEffect();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy();
    }

    EVERY_N_MILLISECONDS(REFRESH_RATE_IN_MILLIS) {
        display.render();
    }
}

void changeEffect() {
    //TODO add stack modifier, each pixel stacks on the previous one + reverse
    //TODO add swipe effect, one colour slides over the previous one
    //TODO add word dwell modifier, dwell on each word for a while
    //TODO add chase with trail modifier, like ping pong but with a trail
    //TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
    display.changeEffect(effectFactories);
}
