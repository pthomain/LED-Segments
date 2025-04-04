#include "SimpleRenderer.h"
#include "engine/render/PixelMapper.h"

SimpleRenderer::SimpleRenderer(
    const DisplaySpec &displaySpec,
    PixelMapper *pixelMapper,
    const String &name
) : Renderer(displaySpec, name),
    effectArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    pixelMapper(pixelMapper) {
}

void SimpleRenderer::changeEffect(std::shared_ptr<Effect> effect) {
    currentEffect = effect;
    frameIndex = 0;
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        if constexpr (IS_DEBUG) Serial.println("No effect on " + name);
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

std::shared_ptr<Effect> SimpleRenderer::getEffect() {
    return currentEffect;
}
