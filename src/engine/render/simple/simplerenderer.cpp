#include "simplerenderer.h"
#include "engine/displayspec.h"

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
    frameIndex = 0;
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        Serial.println("No effect on " + name);
        return;
    }

    EffectContext context = currentEffect->effectContext;
    uint16_t layoutIndex = context.layoutIndex;
    uint16_t nbSegments = displaySpec.nbSegments(layoutIndex);

    //TODO calculate interpolated value

    for (uint8_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        uint16_t segmentSize = displaySpec.segmentSize(layoutIndex, segmentIndex);

        //TODO this isn't compatible with the Fibonacci pixel optimisation
        if (segmentIndex % context.segmentSelectionModulo != 0) {
            for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
                effectArray[pixelIndex] = 0;
            }
        } else {
            uint16_t mirrorSize = getMirrorSize(context.mirror, segmentSize);
            currentEffect->fillArray(effectArray, mirrorSize, frameIndex);
            applyMirror(context.mirror, effectArray, segmentSize);
        }

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec.setColour(
                    layoutIndex,
                    segmentIndex,
                    pixelIndex,
                    frameIndex,
                    outputArray,
                    effectArray[pixelIndex]
            );
        }
    }

    frameIndex++;
}