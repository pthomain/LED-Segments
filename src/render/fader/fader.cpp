#include "fader.h"
#include "config.h"
#include "BitArray.h"
#include "effects/effect.h"
#include "displayspec/specs/TestSpec.h"
#include "render/simplerenderer/simplerenderer.h"

Fader::Fader(
        std::shared_ptr<DisplaySpec> displaySpec,
        const uint16_t effectArraySize
) : Renderer(displaySpec, effectArraySize),
    firstRenderer(new SimpleRenderer(displaySpec, effectArraySize)),
    secondRenderer(new SimpleRenderer(displaySpec, effectArraySize)),
    blendingArray(new CRGB[displaySpec->totalLeds]) {

    for (uint16_t i = 0; i < displaySpec->totalLeds; i++) {
        blendingArray[i] = CRGB::Black;
    }
};

void Fader::changeEffect(std::unique_ptr<Effect> effect) {
    crossFadeStep = TRANSITION_DURATION_IN_FRAMES;

    if (isFirstEffectRendering) secondRenderer->changeEffect(std::move(effect));
    else firstRenderer->changeEffect(std::move(effect));

    isFirstEffectRendering = !isFirstEffectRendering;
}

void Fader::render(CRGB *outputArray) {
    if (crossFadeStep == 0) {
        if (isFirstEffectRendering) firstRenderer->render(outputArray);
        else secondRenderer->render(outputArray);
        return;
    }

    firstRenderer->render(outputArray);
    secondRenderer->render(blendingArray);

    float percent = crossFadeStep / TRANSITION;
    uint8_t overlay = 255 * (isFirstEffectRendering ? percent : 1 - percent);

    for (int i = 0; i < displaySpec->totalLeds; i++) {
        outputArray[i] = blend(
                outputArray[i],
                blendingArray[i],
                overlay
        );
    }

    crossFadeStep = max(0, crossFadeStep - 1);
}
