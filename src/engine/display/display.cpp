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
        const uint8_t effectDurationsInSecs,
        const int16_t transitionDurationInMillis,
        const uint8_t fps,
        const uint8_t *freePinsForEntropy,
        const uint8_t nbPinsForEntropy
) : effectDurationsInSecs(effectDurationsInSecs),
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

uint8_t index = 0;

void Display::changeEffect() {
    const auto effectFactoryIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectFactoryIndex);
    const auto layoutIndex = index;//random8(displaySpec.nbLayouts());
    const auto mirror = MIRROR_NONE;//ALL_MIRRORS[random8(ALL_MIRRORS.size())];

    renderer->changeEffect(effectFactory(
            EffectContext(
                    displaySpec.isCircular(),
                    layoutIndex,
                    effectIndex,
                    PALETTES[random8(PALETTES.size())],
                    mirror,
                    LINEAR,
                    Transition::SLIDE_LTR,
                    MIRROR_NONE
            )
    ));

    effectIndex++;
    index = (index + 1) % displaySpec.nbLayouts();
    if (index == 0)Serial.println();
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
        addEntropy(freePinsForEntropy, nbPinsForEntropy);
    }
}