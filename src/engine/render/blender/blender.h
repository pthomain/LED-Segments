#ifndef LED_SEGMENTS_BLENDER_H
#define LED_SEGMENTS_BLENDER_H

#include "FastLED.h"
#include "engine/effect/effect.h"
#include "engine/render/renderer.h"
#include "engine/render/pixelmapper.h"
#include "engine/displayspec/displayspec.h"
#include "memory"
#include "WString.h"

class Blender : public Renderer, PixelMapper {
    std::unique_ptr<Renderer> runningRenderer = nullptr;
    std::unique_ptr<Renderer> blendingRenderer = nullptr;

    CRGB *runningArray;
    CRGB *blendingArray;

    CRGB *transitionSegmentArray;
    CRGB *transitionArray;
    float transitionStep = -1;

    //todo shared pointer
    const EffectContext *currentEffectContext = nullptr;

    const String runningRendererName = "runningRenderer";
    const String blendingRendererName = "blendingRenderer";

    const uint16_t refreshRateInMillis;
    const uint16_t transitionDurationInMillis;
    const uint16_t transitionDurationInFrames = transitionDurationInMillis / refreshRateInMillis;

    void fillTransition(float transitionPercent) const;

    void applyTransition(CRGB *outputArray, float transitionPercent) const;

public :
    explicit Blender(
        const DisplaySpec &displaySpec,
        const String &name,
        const uint16_t refreshRateInMillis,
        const uint16_t transitionDurationInMillis
    );

    void changeEffect(std::shared_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    std::shared_ptr<Effect> getEffect() override;

    void mapPixels(
        const String &rendererName,
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t segmentSize,
        const uint16_t frameIndex,
        CRGB *outputArray,
        CRGB *effectArray
    ) override;

    ~Blender() override {
        delete[] runningArray;
        delete[] blendingArray;
        delete[] transitionSegmentArray;
        delete[] transitionArray;
        if (runningRenderer) runningRenderer.reset();
        if (blendingRenderer) blendingRenderer.reset();
    }
};

#endif //LED_SEGMENTS_BLENDER_H
