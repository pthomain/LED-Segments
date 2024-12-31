#include "display.h"

Display::Display(
        uint16_t totalLeds,
        std::vector<std::vector<Segment *>> layouts
) : outputArray(new CRGB[totalLeds]),
    layouts(std::move(layouts)) {
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(outputArray, totalLeds);
    FastLED.clear(true);
    FastLED.show();

    // Create effect array with max length of any segment across all layouts
    uint16_t effectArraySize = 0;
    for (const auto &layout: layouts) {
        for (const auto &segment: layout) {
            effectArraySize = max(effectArraySize, segment->size);
        }
    }

    renderer = DISABLE_FADING ? (Renderer *) new SimpleRenderer(effectArraySize) : new Fader(effectArraySize);
}

void Display::changeEffect(
        const std::vector<EffectFactory> &effectFactories
) {
    const auto effectIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectIndex);
    const auto layoutIndex = random8(layouts.size());
    const auto layout = layouts.at(layoutIndex);
    const auto mirror = ALL_MIRRORS[random8(3)];

    const auto effect = effectFactory(
            EffectContext(
                    MIRROR_NONE,
                    effectIndex,
                    0
            )
    );

    renderer->changeEffect(
            effect,
            layout
    );
}

void Display::render() {
    renderer->render(outputArray);
}