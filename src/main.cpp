#include <FastLED.h>
#include "config.h"
#include "effects/party/partyeffect.h"
#include "utils/seedgenerator.h"
#include "structure/display.h"
#include "displayspec/specs/phrase/PhraseSpec.h"
#include "effects/noise/noiseeeffect.h"
#include "displayspec/specs/FibonacciSpec.h"

//TODO add stack modifier, each pixel stacks on the previous one + reverse
//TODO add swipe effect, one colour slides over the previous one
//TODO add word dwell modifier, dwell on each word for a while
//TODO add chase with trail modifier, like ping pong but with a trail
//TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
//TODO add a Composite effect that picks a different effect for each segment or the same effect but a different palette

Display *display;

void setup() {
    Serial.begin(9600);
    addEntropy();
    delay(2000);

    //TODO move API files to lib folder
    auto *actualSpec = new FibonacciSpec();
    DisplaySpec &displaySpec = *actualSpec;

    display = new Display(
            displaySpec,
            {
                    PartyEffect::factory,
                    NoiseEffect::factory
            }
    );
    display->changeEffect();
    display->render();
}

void loop() {
    EVERY_N_SECONDS(EFFECT_DURATION_IN_SECONDS) {
        display->changeEffect();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy();
    }

    EVERY_N_MILLISECONDS(REFRESH_RATE_IN_MILLIS) {
        display->render();
    }
}
