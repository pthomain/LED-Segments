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
    runningArray(new CRGB[displaySpec.nbLeds()]{}),
    blendingArray(new CRGB[displaySpec.nbLeds()]{}),
    transitionSegmentArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    transitionArray(new CRGB[displaySpec.nbLeds()]{}),
    refreshRateInMillis(refreshRateInMillis),
    transitionDurationInMillis(transitionDurationInMillis) {
    //Initialisation of both renderers needs to be done here, moving it to property setters causes issues
    runningRenderer = std::make_unique<SimpleRenderer>(displaySpec, (PixelMapper *) this, runningRendererName);
    blendingRenderer = std::make_unique<SimpleRenderer>(displaySpec, (PixelMapper *) this, blendingRendererName);
}

void Blender::changeEffect(std::shared_ptr<Effect> effect) {
    currentEffectContext = &effect->effectContext;

    Serial.println(
        "Layout: " + displaySpec.layoutName(currentEffectContext->layoutIndex)
        + "\t\tEffect: " + effect->name()
        + "\t\tMirror: " + getMirrorName(effect->effectContext.mirror)
    );

    if (runningRenderer->getEffect() != nullptr) {
        transitionStep = transitionDurationInFrames;
        for (uint16_t pixelIndex = 0; pixelIndex < displaySpec.maxSegmentSize(); pixelIndex++) {
            transitionSegmentArray[pixelIndex] = 0;
        }
        blendingRenderer->changeEffect(effect);
    } else {
        runningRenderer->changeEffect(effect);
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
        auto blendedColour = blend(
            runningArray[pixelIndex],
            blendingArray[pixelIndex],
            transitionArray[pixelIndex].r
        );

        //TODO handle alpha
        outputArray[pixelIndex] = blendedColour;
    }
}

void Blender::render(CRGB *outputArray) {
    if (transitionStep < 0) {
        runningRenderer->render(outputArray);
    } else {
        runningRenderer->render(outputArray);
        blendingRenderer->render(outputArray);

        const float transitionPercent = 1 - (transitionStep / (float) transitionDurationInFrames);
        applyTransition(outputArray, transitionPercent);
        transitionStep = max(-1, transitionStep - 1);

        if (transitionStep < 0) {
            runningRenderer->changeEffect(blendingRenderer->getEffect());
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

std::shared_ptr<Effect> Blender::getEffect() {
    return nullptr; //NOOP
}
