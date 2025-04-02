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

private:
    Renderer *firstRenderer = nullptr;
    Renderer *secondRenderer = nullptr;
    CRGB *firstArray = nullptr;
    CRGB *secondArray = nullptr;

    CRGB *transitionSegmentArray;
    CRGB *transitionArray;
    float transitionStep = -1;
    const EffectContext *currentEffectContext = nullptr;

    const String firstRendererName = "firstRenderer";
    const String secondRendererName = "secondRenderer";

    void applyCustomTransition(CRGB *outputArray, float transitionPercent);

private :
    const uint16_t refreshRateInMillis;
    const uint16_t transitionDurationInMillis;
    const uint16_t transitionDurationInFrames = transitionDurationInMillis / refreshRateInMillis;
    bool isFirstEffectRendering = true;

public :

    explicit Blender(
            const DisplaySpec &displaySpec,
            const String &name,
            const uint16_t refreshRateInMillis,
            const uint16_t transitionDurationInMillis
    );

    bool hasEffect() override;

    void changeEffect(std::unique_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

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
        delete[] firstArray;
        delete[] secondArray;
        delete[] transitionSegmentArray;
        delete[] transitionArray;
        delete firstRenderer;
        delete secondRenderer;
    }
};

#endif //LED_SEGMENTS_BLENDER_H
