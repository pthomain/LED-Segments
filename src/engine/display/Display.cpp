#include "Display.h"
#include "engine/render/simple/SimpleRenderer.h"
#include "engine/render/blender/Blender.h"
#include "utils/SeedGenerator.h"
#include "engine/render/simple/SimplePixelMapper.h"
#include <FastLED.h>

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
    CRGB *outputArray,
    const DisplaySpec &displaySpec,
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
    outputArray(outputArray),
    freePinsForEntropy(freePinsForEntropy),
    nbPinsForEntropy(nbPinsForEntropy) {
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    addEntropy(freePinsForEntropy, nbPinsForEntropy);
    changeEffect();
    render();
}

void Display::changeEffect() {
    const auto catalog = displaySpec.catalog();
    const auto effects = catalog.supportedEffects();

    if (effects.empty()) {
        if constexpr (IS_DEBUG) Serial.print("No effects provided");
        return;
    }
    const auto effectFactoryIndex = random8(effects.size());
    const auto effectFactory = effects.at(effectFactoryIndex);

    const auto layoutIndex = random16(displaySpec.nbLayouts());
    const auto mirrorableLayouts = catalog.matchLayouts(MIRRORABLE);

    const auto effectMirror = contains(mirrorableLayouts, layoutIndex)
                                  ? ALL_MIRRORS[random8(ALL_MIRRORS.size())]
                                  : MIRROR_NONE;

    const auto transition = ALL_TRANSITIONS[random8(ALL_TRANSITIONS.size())];

    const auto transitionLayouts = catalog.matchLayouts(TRANSITIONABLE);
    const auto transitionLayoutIndex = transitionLayouts.at(random8(transitionLayouts.size()));

    const auto transitionMirror = contains(mirrorableLayouts, transitionLayoutIndex) && transition != FADE
                                      ? ALL_MIRRORS[random8(ALL_MIRRORS.size())]
                                      : MIRROR_NONE;

    //TODO highlight

    const auto palette = PALETTES[random8(PALETTES.size())];

    renderer->changeEffect(effectFactory(
        EffectContext(
            displaySpec.isCircular(),
            layoutIndex,
            effectIndex,
            palette,
            effectMirror,
            transition,
            transitionLayoutIndex,
            transitionMirror
        )
    ));

    effectIndex++;
    // index = (index + 1) % displaySpec.nbLayouts();
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

        if constexpr (IS_DEBUG) Serial.println("Next effect change in " + String(currentEffectDurationsInSecs) + "s");

        changeEffect();
    }

    EVERY_N_MILLISECONDS(refreshRateInMillis) {
        render();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy(freePinsForEntropy, nbPinsForEntropy);
    }
}
