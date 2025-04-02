#include "blender.h"
#include "engine/effect/effect.h"
#include "engine/render/simple/simplerenderer.h"
#include "engine/effect/transition.h"

Blender::Blender(
        const DisplaySpec &displaySpec,
        const String &name,
        const uint16_t refreshRateInMillis,
        const uint16_t transitionDurationInMillis
) : Renderer(displaySpec, name),
    firstRenderer(new SimpleRenderer(displaySpec, (PixelMapper *) this, firstRendererName)),
    secondRenderer(new SimpleRenderer(displaySpec, (PixelMapper *) this, secondRendererName)),
    firstArray(new CRGB[displaySpec.nbLeds()]{}),
    secondArray(new CRGB[displaySpec.nbLeds()]{}),
    transitionArray(new uint8_t[displaySpec.maxSegmentSize()]{}),
    refreshRateInMillis(refreshRateInMillis),
    transitionDurationInMillis(transitionDurationInMillis) {};

bool Blender::hasEffect() {
    return firstRenderer->hasEffect() || secondRenderer->hasEffect();
}

void Blender::changeEffect(std::unique_ptr<Effect> effect) {
    currentEffectContext = &effect->effectContext;

    if (isFirstEffectRendering && firstRenderer->hasEffect()) secondRenderer->changeEffect(std::move(effect));
    else firstRenderer->changeEffect(std::move(effect));

    if (secondRenderer->hasEffect()) transitionStep = transitionDurationInFrames;
}

void Blender::render(CRGB *outputArray) {
    if (transitionStep < 0) {
        if (isFirstEffectRendering && firstRenderer->hasEffect()) firstRenderer->render(outputArray);
        else if (secondRenderer->hasEffect()) secondRenderer->render(outputArray);
    } else {
        if (!firstRenderer->hasEffect() && !secondRenderer->hasEffect()) {
            Serial.println("No effect on both renderers");
            return;
        }

        firstRenderer->render(outputArray);
        secondRenderer->render(outputArray);

        transitionStep = max(-1, transitionStep - 1);
        if (transitionStep < 0) isFirstEffectRendering = !isFirstEffectRendering;
    }
}

void Blender::mapPixels(
        const String &rendererName,
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t segmentSize,
        const uint16_t frameIndex,
        CRGB *outputArray,
        CRGB *effectArray
) {
    if (effectArray == nullptr) {
        Serial.println("Effect array is null for renderer " + rendererName + " with transitionStep == 0");
        return; //should not happen
    }

    CRGB *rendererOutputArray = transitionStep < 0 ? outputArray
                                                   : rendererName == firstRendererName ? firstArray : secondArray;

    for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
        displaySpec.setColour(
                layoutIndex,
                segmentIndex,
                pixelIndex,
                frameIndex,
                rendererOutputArray,
                effectArray[pixelIndex]
        );
    }

    if (transitionStep >= 0 && rendererName == secondRendererName) {
        const float transitionPercent = transitionStep / (float) transitionDurationInFrames;

        applyTransition(
                currentEffectContext->transition,
                currentEffectContext->transitionMirror,
                transitionArray,
                segmentSize,
                1 - transitionPercent
        );

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            CRGB blendedPixel = blend(
                    isFirstEffectRendering ? firstArray[pixelIndex] : secondArray[pixelIndex],
                    isFirstEffectRendering ? secondArray[pixelIndex] : firstArray[pixelIndex],
                    transitionArray[pixelIndex]
            );
            displaySpec.setColour(
                    layoutIndex,
                    segmentIndex,
                    pixelIndex,
                    frameIndex,
                    outputArray,
                    blendedPixel
            );
        }
    }
}