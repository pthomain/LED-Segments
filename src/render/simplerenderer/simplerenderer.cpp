#include "simplerenderer.h"
#include "displayspec/displayspec.h"

SimpleRenderer::SimpleRenderer(
        const DisplaySpec &displaySpec,
        const String &name
) : Renderer(displaySpec, name),
    effectArray(new CRGB[displaySpec.maxSegmentSize()]) {
    for (uint16_t i = 0; i < displaySpec.maxSegmentSize(); i++) {
        effectArray[i] = CRGB::Black;
    }
}

void SimpleRenderer::changeEffect(std::unique_ptr<Effect> effect) {
    currentEffect = std::move(effect);
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        Serial.println("No effect on " + name);
        return;
    }

    uint16_t layoutIndex = currentEffect->effectContext.layoutIndex;

    uint16_t nbSegments = displaySpec.nbSegments(layoutIndex);

    for (uint8_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        uint16_t nbPixels = displaySpec.segmentSize(layoutIndex, segmentIndex);

        currentEffect->fillArray(effectArray, nbPixels);

        for (uint16_t pixelIndex = 0; pixelIndex < nbPixels; pixelIndex++) {
            auto colour = effectArray[pixelIndex];
            displaySpec.setColour(layoutIndex, segmentIndex, pixelIndex, outputArray, colour);
        }
    }
}