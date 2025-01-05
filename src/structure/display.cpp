#include "display.h"
#include "config.h"
#include "render/simplerenderer/simplerenderer.h"
#include "render/fader/fader.h"

Display::Display(
        std::shared_ptr<DisplaySpec> displaySpec,
        std::vector<EffectFactory> effectFactories
) : displaySpec(displaySpec),
    renderer(DISABLE_FADING
             ? std::unique_ptr<Renderer>(new SimpleRenderer(displaySpec))
             : std::unique_ptr<Renderer>(new Fader(displaySpec))
    ),
    effectFactories(std::move(effectFactories)),
    outputArray(new CRGB[displaySpec->nbLeds()]) {

    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(outputArray, displaySpec->nbLeds());
    FastLED.setBrightness(10);
    FastLED.clear(true);
    FastLED.show();
}

void Display::changeEffect() {
    const auto effectIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectIndex);
    const auto layoutIndex = random8(displaySpec->nbLayouts());
    const auto mirror = ALL_MIRRORS[random8(3)];

    Serial.println(
            "Effect Index: " + String(effectIndex) +
            ", Layout Index: " + String(layoutIndex) +
            ", Mirror: " + String(mirror)
    );

    renderer->changeEffect(effectFactory(
            EffectContext(
                    layoutIndex,
                    MIRROR_NONE,
                    effectIndex
            )
    ));
}

void Display::render() {
    renderer->render(outputArray);
    FastLED.show();
}