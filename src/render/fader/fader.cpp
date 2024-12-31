#include "fader.h"
#include "config.h"
#include "BitArray.h"
#include "effects/effect.h"
#include "render/simplerenderer/simplerenderer.h"

Fader::Fader(const uint16_t effectArraySize) :
        Renderer(effectArraySize),
        firstRenderer(new SimpleRenderer(effectArraySize)),
        secondRenderer(new SimpleRenderer(effectArraySize)) {
    blendingArray = new CRGB[effectArraySize];
};

void Fader::changeEffect(
        std::shared_ptr<Effect> effect,
        const std::vector<Segment *> &layout
) {
    crossFadeMax = TRANSITION_DURATION_IN_FRAMES;
    crossFadeStep = TRANSITION_DURATION_IN_FRAMES;

    if (isFirstEffectRendering) secondRenderer->changeEffect(effect, layout);
    else firstRenderer->changeEffect(effect, layout);
}

void Fader::render(CRGB *outputArray) {
    if (crossFadeStep == 0) {
        if (isFirstEffectRendering) firstRenderer->render(outputArray);
        else secondRenderer->render(outputArray);
        return;
    }

    firstRenderer->render(outputArray);
    secondRenderer->render(blendingArray);

    fract8 overlay = (fract8) ((1 + crossFadeMax - crossFadeStep) / crossFadeMax) * 255;
    if (!isFirstEffectRendering) overlay = 255 - overlay;

    for (int i = 0; i < effectArraySize; i++) {
        outputArray[i] = blend(
                outputArray[i],
                blendingArray[i],
                overlay
        );
    }
    crossFadeStep--;

    if (crossFadeStep == 0) isFirstEffectRendering = !isFirstEffectRendering;
}
