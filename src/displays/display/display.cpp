#include "FastLED.h"
#include <utility>
#include <vector>
#include <sstream>
#include "effects/effect.h"
#include "effects/mirror/mirror.h"
#include "display.h"

Display::Display(uint16_t totalLeds, std::vector<Segment *> layouts) :
        layouts(std::move(layouts)),
        totalLeds(totalLeds),
        allLeds(new CRGB[totalLeds]),
        fader(allLeds, totalLeds) {
    CFastLED::addLeds<WS2812B, 9, GRB>(allLeds, totalLeds);
    FastLED.clear(true);
    FastLED.show();
}

void Display::changeEffects(
        const uint16_t transitionDurationInFrames,
        const std::vector<std::function<Effect *(const EffectContext &effectContext)>> &effectFactories
) {
    const auto &effectFactory = effectFactories.at(random8(effectFactories.size()));
    auto effectsAndPixels = std::vector<std::pair<Effect *, std::vector<Pixel *>>>();

    auto layoutIndex = random8(layouts.size());
    auto layout = layouts.at(layoutIndex);
    uint8_t floor = random8(layout->bottom());
    uint8_t ceiling = random8(floor);

    Mirror mirror = ALL_MIRRORS[random8(3)];
    std::ostringstream oss;
    oss << "Layout Index: " << static_cast<int>(layoutIndex)
        << ", Floor: " << static_cast<int>(floor)
        << ", Ceiling: " << static_cast<int>(ceiling)
        << ", Mirror: " << mirrorToString(mirror);
    Serial.println(oss.str().c_str());

    auto slicedPixels = layout->slicePixels(ceiling, floor);

    for (uint16_t effectIndex = 0; effectIndex < slicedPixels.size(); effectIndex++) {
        auto pixels = slicedPixels.at(effectIndex);
        auto effect = effectFactory(
                EffectContext(
                        MIRROR_NONE,
                        pixels.size(),
                        effectIndex,
                        0
                )
        );

        effectsAndPixels.emplace_back(effect, pixels);
    }

    fader.applyEffects(
            effectsAndPixels,
            transitionDurationInFrames
    );
}

void Display::render() {
    fader.render();
    FastLED.show();
}
