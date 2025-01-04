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

void Fader::changeEffect(Effect *effect) {
    crossFadeMax = TRANSITION_DURATION_IN_FRAMES;
    crossFadeStep = TRANSITION_DURATION_IN_FRAMES;

    if (isFirstEffectRendering) secondRenderer->changeEffect(effect);
    else firstRenderer->changeEffect(effect);
}

void Fader::render(DisplaySpec *displaySpec, CRGB *outputArray) {
    if (crossFadeStep == 0) {
        if (isFirstEffectRendering) firstRenderer->render(displaySpec, outputArray);
        else secondRenderer->render(displaySpec, outputArray);
        return;
    }

    firstRenderer->render(displaySpec, outputArray);
    secondRenderer->render(displaySpec, blendingArray);

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
