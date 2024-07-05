#include <FastLED.h>
#include "utils/utils.h"
#include "structure/display.h"
#include "effects/effect.h"
#include "structure/cluster.h"
#include <memory>
#include "effects/partyeffect.h"
#include "effects/redeffect.h"

#define TRANSITION_DURATION_IN_MILLIS 1000
#define FPS 30
#define REFRESH_RATE_IN_MILLIS (1000 / FPS)
#define EFFECT_DURATION_IN_SECONDS 5

#define TRANSITION_DURATION_IN_MILLIS 1000
#define TRANSITION_DURATION_IN_FRAMES TRANSITION_DURATION_IN_MILLIS / REFRESH_RATE_IN_MILLIS

Display *display = nullptr;
std::vector<std::pair<std::function<Effect *(const Section &, Mirror)>, Variation>> effectFactories;

void changeEffect();

void setup() {
    Serial.begin(9600);

    effectFactories = {
            std::make_pair(PartyEffect::factory, PartyEffect::variation),
            std::make_pair(RedEffect::factory, RedEffect::variation)
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
    display->pickNewEffect(
            TRANSITION_DURATION_IN_FRAMES,
            effectFactories
    );
}
