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
    runningRenderer(new SimpleRenderer(displaySpec, (PixelMapper *) this, runningRendererName)),
    blendingRenderer(new SimpleRenderer(displaySpec, (PixelMapper *) this, blendingRendererName)),
    runningArray(new CRGB[displaySpec.nbLeds()]{}),
    blendingArray(new CRGB[displaySpec.nbLeds()]{}),
    transitionSegmentArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    transitionArray(new CRGB[displaySpec.nbLeds()]{}),
    refreshRateInMillis(refreshRateInMillis),
    transitionDurationInMillis(transitionDurationInMillis) {
};

bool Blender::hasEffect() {
    return runningRenderer->hasEffect() || blendingRenderer->hasEffect();
}

void Blender::changeEffect(std::unique_ptr<Effect> effect) {
    currentEffectContext = &effect->effectContext;

    if (runningRenderer->hasEffect()) {
        blendingRenderer->changeEffect(std::move(effect));
        transitionStep = transitionDurationInFrames;
    } else {
        runningRenderer->changeEffect(std::move(effect));
    }
}

void Blender::fillTransition(float transitionPercent) const {
    auto transitionSegments = displaySpec.nbSegments(currentEffectContext->transitionLayoutIndex);

    for (auto segmentIndex = 0; segmentIndex < transitionSegments; segmentIndex++) {
        auto segmentSize = displaySpec.nbPixels(currentEffectContext->transitionLayoutIndex, segmentIndex);
        uint16_t transitionMirrorSize = getMirrorSize(currentEffectContext->transitionMirror, segmentSize);

        fillTransitionArray(
            currentEffectContext->transition,
            currentEffectContext->transitionMirror,
            transitionSegmentArray,
            transitionMirrorSize,
            transitionPercent
        );

        applyMirror(currentEffectContext->transitionMirror, transitionSegmentArray, segmentSize);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec.setColour(
                currentEffectContext->transitionLayoutIndex,
                segmentIndex,
                pixelIndex,
                0,
                transitionArray,
                transitionSegmentArray[pixelIndex]
            );
        }
    }
}

void Blender::applyTransition(
    CRGB *outputArray,
    float transitionPercent
) const {
    fillTransition(transitionPercent);

    for (uint16_t pixelIndex = 0; pixelIndex < displaySpec.nbLeds(); pixelIndex++) {
        auto blendedColour = transitionArray[pixelIndex] == CRGB::White
                                 ? blendingArray[pixelIndex]
                                 : runningArray[pixelIndex];

        //TODO handle alpha
        outputArray[pixelIndex] =  blendedColour;
    }
}

void Blender::render(CRGB *outputArray) {
    if (transitionStep < 0) {
        if (runningRenderer->hasEffect()) runningRenderer->render(outputArray);
        else Serial.println("No effect on running renderer with transition step < 0");
    } else {
        if (!runningRenderer->hasEffect()) {
            Serial.println("No effect on running renderer with transition step >= 0");
            return;
        }
        if (!blendingRenderer->hasEffect()) {
            Serial.println("No effect on blending renderer with transition step >= 0");
            return;
        }

        runningRenderer->render(outputArray);
        blendingRenderer->render(outputArray);

        const float transitionPercent = 1 - (transitionStep / (float) transitionDurationInFrames);
        applyTransition(outputArray, transitionPercent);
        transitionStep = max(-1, transitionStep - 1);

        if (transitionStep < 0) {
            runningRenderer->changeEffect(std::move(blendingRenderer->handoverEffect()));
        }
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

    CRGB *rendererOutputArray = transitionStep < 0
                                    ? outputArray
                                    : rendererName == runningRendererName
                                          ? runningArray
                                          : blendingArray;

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

std::unique_ptr<Effect> Blender::handoverEffect() {
    return nullptr; //NOOP
}