#include "simplerenderer.h"
#include "displayspec/displayspec.h"

SimpleRenderer::SimpleRenderer(const uint16_t effectArraySize) :
        Renderer(effectArraySize),
        effectArray(new CRGB[effectArraySize]) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = CRGB::Black;
    }
}

void SimpleRenderer::changeEffect(Effect *effect) {
    if (currentEffect != nullptr) {
        delete currentEffect;
        currentEffect = nullptr;
    }

    currentEffect = effect;
}

void SimpleRenderer::render(DisplaySpec *displaySpec, CRGB *outputArray) {
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
