#include "display.h"
#include "engine/render/simple/simplerenderer.h"
#include "engine/render/fader/fader.h"
#include "engine/utils/seedgenerator.h"
#include <FastLED.h>

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
        CRGB *outputArray,
        const DisplaySpec &displaySpec,
        const std::vector<EffectFactory> effectFactories,
        const uint8_t brightness,
        const uint8_t effectDurationsInSecs,
        const uint8_t fps,
        const int16_t transitionDurationInMillis
) : effectDurationsInSecs(effectDurationsInSecs),
    fps(fps),
    transitionDurationInMillis(transitionDurationInMillis),
    refreshRateInMillis(1000 / fps),
    displaySpec(std::move(displaySpec)),
    renderer(std::unique_ptr<Renderer>(
            transitionDurationInMillis < 1
            ? (Renderer *) new SimpleRenderer(displaySpec, "simple")
            : new Fader(displaySpec, "fader", refreshRateInMillis, transitionDurationInMillis)
    )),
    effectFactories(std::move(effectFactories)),
    outputArray(outputArray) {

    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    addEntropy();
    changeEffect();
    render();
}

uint16_t index = 0;

void Display::changeEffect() {
    const auto effectIndex = 0;//random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectIndex);
    const auto layoutIndex = 0;//random8(displaySpec.nbLayouts());
    const auto mirror = ALL_MIRRORS[index];

    Serial.println(
            "Layout: " + displaySpec.layoutName(layoutIndex) +
            "\t\tMirror: " + getMirrorName(mirror)
    );

    renderer->changeEffect(effectFactory(
            EffectContext(
                    layoutIndex,
                    PALETTES[random8(PALETTES.size())],
                    mirror,
                    LINEAR
            )
    ));

    index = ++index % ALL_MIRRORS.size();
}

void Display::render() {
    renderer->render(outputArray);
    FastLED.show();
}

void Display::loop() {
    EVERY_N_SECONDS(effectDurationsInSecs) {
        changeEffect();
    }
    EVERY_N_MILLISECONDS(refreshRateInMillis) {
        render();
    }
    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy();
    }
}