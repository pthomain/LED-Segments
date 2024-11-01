#include "canvas.h"
#include "structure/pixel/Pixel.h"
#include "structure/segment/Segment.h"

void Canvas::applyEffects(
        const std::vector<std::pair<Effect *, std::vector<Pixel *>>>& effectsAndPixels
) {
    for (const auto &pair: effectsAndPixels) {
        auto effect = pair.first;
        auto pixels = pair.second;

        effect->fillArray(effectBufferArray);

        for (auto index = 0; index < pixels.size(); index++) {
            auto pixel = pixels[index];
            pixel->setColour(allEffectsArray, effectBufferArray[index]);
        }
    }
}

void Canvas::render(CRGB *outputArray) {
    for (int i = 0; i < totalLeds; i++) {
        outputArray[i] = allEffectsArray[i];
    }
}
