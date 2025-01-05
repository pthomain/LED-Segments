#include "simplerenderer.h"
#include "displayspec/displayspec.h"

SimpleRenderer::SimpleRenderer(
        std::shared_ptr<DisplaySpec> displaySpec,
        const uint16_t effectArraySize
) : Renderer(displaySpec, effectArraySize),
    effectArray(new CRGB[effectArraySize]) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = CRGB::Black;
    }
}

void SimpleRenderer::changeEffect(std::unique_ptr<Effect> effect) {
    currentEffect = std::move(effect);
}

void SimpleRenderer::render(CRGB *outputArray) {
    auto layoutIndex = currentEffect->effectContext.layoutIndex;
    for (uint8_t segmentIndex = 0; segmentIndex < displaySpec->nbSegments(layoutIndex); segmentIndex++) {
        uint16_t nbPixels = displaySpec->segmentSize(layoutIndex, segmentIndex);

        currentEffect->fillArray(effectArray, nbPixels);

        for (uint16_t pixelIndex = 0; pixelIndex < nbPixels; pixelIndex++) {
            auto colour = effectArray[pixelIndex];
            displaySpec->setColour(layoutIndex, segmentIndex, pixelIndex, outputArray, colour);
        }
    }
}
