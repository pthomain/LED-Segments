#include "fader.h"
#include "render/canvas.h"

#define DISABLE_FADING false

void Fader::applyConfig(
        EffectConfig *effectConfig,
        const uint16_t transitionDurationInFrames
) {
    if (DISABLE_FADING) {
        firstCanvas.applyConfig(effectConfig);
        return;
    }
    crossFadeMax = transitionDurationInFrames;
    crossFadeStep = transitionDurationInFrames;

    if (isFirstCanvasRendering) {
        secondCanvas.applyConfig(effectConfig);
    } else {
        firstCanvas.applyConfig(effectConfig);
    }
}

void Fader::render() {
    if (DISABLE_FADING) {
        firstCanvas.render(outputArray);
        return;
    }

    if (crossFadeStep == 0) {
        if (isFirstCanvasRendering) {
            firstCanvas.render(firstCanvasOutputArray);
            for (int i = 0; i < totalLeds; i++) {
                outputArray[i] = firstCanvasOutputArray[i];
            }
        } else {
            secondCanvas.render(secondCanvasOutputArray);
            for (int i = 0; i < totalLeds; i++) {
                outputArray[i] = secondCanvasOutputArray[i];
            }
        }
    } else {
        firstCanvas.render(firstCanvasOutputArray);
        secondCanvas.render(secondCanvasOutputArray);

        fract8 overlay = ((float) (1 + crossFadeMax - crossFadeStep) / crossFadeMax) * 255;

        if (!isFirstCanvasRendering) {
            overlay = 255 - overlay;
        }

        for (int i = 0; i < totalLeds; i++) {
            outputArray[i] = blend(
                    firstCanvasOutputArray[i],
                    secondCanvasOutputArray[i],
                    overlay
            );
        }
        crossFadeStep--;

        if (crossFadeStep == 0) {
            isFirstCanvasRendering = !isFirstCanvasRendering;
        }
    }
}
