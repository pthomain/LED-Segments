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
    transitionSegmentArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    transitionArray(new CRGB[displaySpec.nbLeds()]{}),
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

void Blender::applyCustomTransition(CRGB *outputArray, float transitionPercent) {
    auto transitionLayoutIndex = 0; //TODO move to context
    auto transitionSegments = displaySpec.nbSegments(transitionLayoutIndex);

    for (auto segmentIndex = 0; segmentIndex < transitionSegments; segmentIndex++) {
        auto segmentSize = displaySpec.nbPixels(transitionLayoutIndex, segmentIndex);
        uint16_t transitionMirrorSize = getMirrorSize(currentEffectContext->transitionMirror, segmentSize);

        applyTransition(
                currentEffectContext->transition,
                currentEffectContext->transitionMirror,
                transitionSegmentArray,
                transitionMirrorSize,
                transitionPercent
        );

        applyMirror(currentEffectContext->transitionMirror, transitionSegmentArray, segmentSize);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec.setColour(
                    transitionLayoutIndex,
                    segmentIndex,
                    pixelIndex,
                    0,
                    transitionArray,
                    transitionSegmentArray[pixelIndex]
            );
        }
    }

    for (uint16_t pixelIndex = 0; pixelIndex < displaySpec.nbLeds(); pixelIndex++) {
        outputArray[pixelIndex] = blend(
                isFirstEffectRendering ? firstArray[pixelIndex] : secondArray[pixelIndex],
                isFirstEffectRendering ? secondArray[pixelIndex] : firstArray[pixelIndex],
                transitionArray[pixelIndex] == CRGB::White ? 255 : 0
        );
    }
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

        const float transitionPercent = 1 - (transitionStep / (float) transitionDurationInFrames);

        applyCustomTransition(outputArray, transitionPercent);

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
        Serial.println("Effect array is null for renderer " + rendererName);
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
}