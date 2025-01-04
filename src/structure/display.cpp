#include "display.h"
#include "config.h"
#include "displayspec/specs/TestSpec.h"
#include "render/simplerenderer/simplerenderer.h"
#include "render/fader/fader.h"

Display::Display(DisplaySpec *displaySpec)
        : displaySpec(displaySpec),
          outputArray(new CRGB[displaySpec->totalLeds]) {
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(outputArray, displaySpec->totalLeds);
    FastLED.setBrightness(10);
    FastLED.clear(true);
    FastLED.show();

    uint16_t maxSegmentSize = 0;

    for (uint8_t layoutIndex = 0; layoutIndex < displaySpec->nbLayouts; layoutIndex++) {
        for (uint8_t segmentIndex = 0; segmentIndex < displaySpec->nbSegments(layoutIndex); segmentIndex++) {
            maxSegmentSize = max(maxSegmentSize, displaySpec->segmentSize(layoutIndex, segmentIndex));
        }
    }

    renderer = DISABLE_FADING ? (Renderer *) new SimpleRenderer(maxSegmentSize) : new Fader(maxSegmentSize);
}

void Display::changeEffect(
        const std::vector<EffectFactory> &effectFactories
) {
    const auto effectIndex = random8(effectFactories.size());
    const auto &effectFactory = effectFactories.at(effectIndex);

    const auto layoutIndex = random8(displaySpec->nbLayouts);
    const auto mirror = ALL_MIRRORS[random8(3)];

    const auto effect = effectFactory(
            EffectContext(
                    layoutIndex,
                    MIRROR_NONE,
                    effectIndex
            )
    );

    renderer->changeEffect(effect);
}

void Display::render() {
    renderer->render(displaySpec, outputArray);
    FastLED.show();
}