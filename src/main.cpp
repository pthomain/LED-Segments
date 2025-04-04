#include "engine/display/Display.h"
#include "specs/phrase/PhraseSpec.h"
#include "specs/fibonacci/FibonacciSpec.h"

//TODO add stack modifier, each pixel stacks on the previous one + reverse
//TODO add swipe effect, one colour slides over the previous one
//TODO add word highlight modifier, increase luminosity on each word for a while
//TODO add chase with trail modifier, like ping pong but with a trail
//TODO for each modifier, allow for highlight (75% brightness for other pixels based on seed%2)
//TODO add a Composite effect that picks a different effect for each segment or the same effect but a different palette

#define LED_PIN 9
#define BRIGHTNESS 10
#define MIN_EFFECT_DURATION_IN_SECONDS 2
#define MAX_EFFECT_DURATION_IN_SECONDS 2

//TODO add effect burst

Display *display;

void setup() {
    if constexpr (IS_DEBUG) {
        Serial.begin(9600);
        delay(1000);
    }

    auto *displaySpec = new PhraseSpec();

    display = Display::create<LED_PIN, GRB>(
        *displaySpec,
        BRIGHTNESS,
        MIN_EFFECT_DURATION_IN_SECONDS,
        MAX_EFFECT_DURATION_IN_SECONDS,
        500,
        30
    );
}

void loop() {
    display->loop();
}
