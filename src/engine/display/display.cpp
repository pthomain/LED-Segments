#include "display.h"
#include "engine/render/simple/simplerenderer.h"
#include "engine/render/blender/blender.h"
#include "engine/utils/seedgenerator.h"
#include <FastLED.h>
#include "engine/render/simple/simplepixelmapper.h"

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
    CRGB *outputArray,
    const DisplaySpec &displaySpec,
    const std::vector<EffectFactory> effectFactories,
    const uint8_t brightness,
    const uint8_t minEffectDurationsInSecs,
    const uint8_t maxEffectDurationsInSecs,
    const int16_t transitionDurationInMillis,
    const uint8_t fps,
    const uint8_t *freePinsForEntropy,
    const uint8_t nbPinsForEntropy
) : minEffectDurationsInSecs(minEffectDurationsInSecs),
    maxEffectDurationsInSecs(maxEffectDurationsInSecs),
    currentEffectDurationsInSecs(random8(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
    fps(fps),
    transitionDurationInMillis(transitionDurationInMillis),
    refreshRateInMillis(1000 / fps),
    displaySpec(std::move(displaySpec)),
    renderer(std::unique_ptr<Renderer>(
        transitionDurationInMillis < 1
            ? (Renderer *) new SimpleRenderer(displaySpec, (PixelMapper *) new SimplePixelMapper(displaySpec), "simple")
            : new Blender(displaySpec, "blender", refreshRateInMillis, transitionDurationInMillis)
    )),
    effectFactories(std::move(effectFactories)),
    outputArray(outputArray),
    freePinsForEntropy(freePinsForEntropy),
    nbPinsForEntropy(nbPinsForEntropy) {
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    addEntropy(freePinsForEntropy, nbPinsForEntropy);
    changeEffect();
    render();
}

// uint8_t index = 0;

void Display::changeEffect() {
    const auto effectFactoryIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectFactoryIndex);
    const auto layoutIndex = random8(displaySpec.nbLayouts());
    const auto mirror = ALL_MIRRORS[random8(ALL_MIRRORS.size())];

    renderer->changeEffect(effectFactory(
        EffectContext(
            displaySpec.isCircular(),
            layoutIndex,
            effectIndex,
            PALETTES[random8(PALETTES.size())],
            mirror,
            LINEAR,
            ALL_TRANSITIONS[random8(ALL_TRANSITIONS.size())],
            displaySpec.transitionLayoutIndexes().at(random16(displaySpec.transitionLayoutIndexes().size())),
            ALL_MIRRORS[random8(ALL_MIRRORS.size())]
        )
    ));

    effectIndex++;
    // index = (index + 1) % displaySpec.nbLayouts();
    // if (index == 0)Serial.println();
}

void Display::render() const {
    renderer->render(outputArray);
    FastLED.show();
}

static uint32_t lastChangeTime = 0;

void Display::loop() {
    if (millis() - lastChangeTime >= currentEffectDurationsInSecs * 1000) {
        lastChangeTime = millis();
        currentEffectDurationsInSecs = random8(minEffectDurationsInSecs, maxEffectDurationsInSecs);
        Serial.println("Next effect change in " + String(currentEffectDurationsInSecs) + "s");
        changeEffect();
    }

    EVERY_N_MILLISECONDS(refreshRateInMillis) {
        render();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy(freePinsForEntropy, nbPinsForEntropy);
    }
}
