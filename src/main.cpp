#include "effects/party/partyeffect.h"
#include "engine/display/display.h"
#include "specs/phrase/PhraseSpec.h"
#include "effects/noise/noiseeeffect.h"
#include "specs/fibonacci/FibonacciSpec.h"
#include "effects/rainbow/rainboweffect.h"
#include "effects/cycle/CycleEffect.h"

//TODO add stack modifier, each pixel stacks on the previous one + reverse
//TODO add swipe effect, one colour slides over the previous one
//TODO add word dwell modifier, dwell on each word for a while
//TODO add chase with trail modifier, like ping pong but with a trail
//TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
//TODO add a Composite effect that picks a different effect for each segment or the same effect but a different palette

#define LED_PIN 9
#define BRIGHTNESS 10
#define EFFECT_DURATION_IN_SECONDS 5

Display *display;

void setup() {
    Serial.begin(9600);
    delay(1000);

    auto *displaySpec = new PhraseSpec();
    display = Display::create<LED_PIN, GRB>(
            *displaySpec,
            {
//                    CycleEffect::factory,
//                    NoiseEffect::factory,
                    PartyEffect::factory,
//                    RainbowEffect::factory,
            },
            BRIGHTNESS,
            EFFECT_DURATION_IN_SECONDS,
            2000,
            30
    );
}

void loop() {
    display->loop();
}
