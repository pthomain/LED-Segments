#include "fader.h"
#include "engine/effect/effect.h"
#include "engine/render/simple/simplerenderer.h"

Fader::Fader(
        const DisplaySpec &displaySpec,
        const String &name,
        const uint16_t refreshRateInMillis,
        const uint16_t transitionDurationInMillis
) : Renderer(displaySpec, name),
    firstRenderer(new SimpleRenderer(displaySpec, "first")),
    secondRenderer(new SimpleRenderer(displaySpec, "second")),
    blendingArray(new CRGB[displaySpec.nbLeds()]),
    refreshRateInMillis(refreshRateInMillis),
    transitionDurationInMillis(transitionDurationInMillis) {

    for (uint16_t i = 0; i < displaySpec.nbLeds(); i++) {
        blendingArray[i] = CRGB::Black;
    }
};

bool Fader::hasEffect() {
    return firstRenderer->hasEffect() || secondRenderer->hasEffect();
}

void Fader::changeEffect(std::unique_ptr<Effect> effect) {
    crossFadeStep = transitionDurationInFrames;

    if (isFirstEffectRendering) secondRenderer->changeEffect(std::move(effect));
    else firstRenderer->changeEffect(std::move(effect));
}

void Fader::render(CRGB *outputArray) {
    if (crossFadeStep == 0) {
        if (isFirstEffectRendering && firstRenderer->hasEffect()) firstRenderer->render(outputArray);
        else if (secondRenderer->hasEffect()) secondRenderer->render(outputArray);
        return;
    }

    if (firstRenderer->hasEffect()) firstRenderer->render(outputArray);

    if (secondRenderer->hasEffect()) {
        secondRenderer->render(blendingArray);

        float percent = crossFadeStep / (float) transitionDurationInFrames;
        uint8_t overlay = 255 * (isFirstEffectRendering ? 1 - percent : percent);

        for (int i = 0; i < displaySpec.nbLeds(); i++) {
            outputArray[i] = blend(
                    outputArray[i],
                    blendingArray[i],
                    overlay
            );
        }
    }

    crossFadeStep = max(0, crossFadeStep - 1);

    if (crossFadeStep == 0) {
        isFirstEffectRendering = !isFirstEffectRendering;
    }
}
