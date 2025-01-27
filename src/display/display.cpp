#include "display/display.h"
#include "config.h"
#include "render/simplerenderer.h"
#include "render/fader.h"

Display::Display(
        const DisplaySpec &displaySpec,
        const std::vector<EffectFactory> effectFactories
) : displaySpec(displaySpec),
    renderer(DISABLE_FADING
             ? std::unique_ptr<Renderer>(new SimpleRenderer(displaySpec, "simple"))
             : std::unique_ptr<Renderer>(new Fader(displaySpec, "fader"))
    ),
    effectFactories(std::move(effectFactories)),
    outputArray(new CRGB[displaySpec.nbLeds()]) {

    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(outputArray, displaySpec.nbLeds());
    FastLED.setBrightness(50);
    FastLED.clear(true);
    FastLED.show();
}

uint8_t *layouts = new uint8_t[2]{
        8, //SEGMENT_SPIRAL_CW_NO_INFLEXION
        12 //SEGMENT_SPIRAL_CCW_NO_INFLEXION
};

uint16_t layoutIndex = 0;

void Display::changeEffect() {
    const auto effectIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectIndex);
//    const auto layoutIndex = 5;//random8(displaySpec->nbLayouts());
    const auto mirror = ALL_MIRRORS[random8(3)];

    Serial.println(
            "Layout: " + String(displaySpec.layoutName(layouts[layoutIndex])) +
            "\t\tEffect Index: " + String(effectIndex) +
            "\t\tMirror: " + String(mirror)
    );

    renderer->changeEffect(effectFactory(
            EffectContext(
                    layouts[layoutIndex],
                    mirror,
                    effectIndex
            )
    ));

    layoutIndex = (layoutIndex + 1) % 2;
    //layoutIndex =(layoutIndex + 1) % displaySpec.nbLayouts();
}

void Display::render() {
    renderer->render(outputArray);
    FastLED.show();
}