#include "simplerenderer.h"
#include "engine/render/pixelmapper.h"

SimpleRenderer::SimpleRenderer(
    const DisplaySpec &displaySpec,
    PixelMapper *pixelMapper,
    const String &name
) : Renderer(displaySpec, name),
    effectArray(new CRGB[displaySpec.maxSegmentSize()]),
    pixelMapper(pixelMapper) {
    for (uint16_t i = 0; i < displaySpec.maxSegmentSize(); i++) {
        effectArray[i] = CRGB::Black;
    }
}

void SimpleRenderer::changeEffect(std::unique_ptr<Effect> effect) {
    currentEffect = std::move(effect);
    frameIndex = 0;

    Serial.println(
        "Layout: " + displaySpec.layoutName(currentEffect->effectContext.layoutIndex)
        + "\t\tEffect: " + currentEffect->name()
        + "\t\tMirror: " + getMirrorName(currentEffect->effectContext.mirror)
    );
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        Serial.println("No effect on " + name);
        return;
    }

    EffectContext context = currentEffect->effectContext;
    uint16_t layoutIndex = context.layoutIndex;
    uint16_t nbSegments = displaySpec.nbSegments(layoutIndex);

    for (uint8_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        uint16_t nbPixels = displaySpec.nbPixels(layoutIndex, segmentIndex);
        uint16_t mirrorSize = getMirrorSize(context.mirror, nbPixels);

        currentEffect->fillArray(effectArray, mirrorSize, frameIndex);
        applyMirror(context.mirror, effectArray, nbPixels);

        pixelMapper->mapPixels(
            name,
            layoutIndex,
            segmentIndex,
            nbPixels,
            frameIndex,
            outputArray,
            effectArray
        );
    }

    frameIndex++;
}

std::unique_ptr<Effect> SimpleRenderer::handoverEffect() {
    auto effect = std::move(currentEffect);
    currentEffect = nullptr;
    return effect;
}
