//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

/*
 * This file is part of LED Segments.
 *
 * LED Segments is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LED Segments is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LED Segments. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Blender.h"
#include "engine/effect/Effect.h"
#include "engine/render/simple/SimplePixelMapper.h"
#include "engine/render/simple/SimpleRenderer.h"

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
    std::shared_ptr<PixelMapper> mapper(this);
    runningRenderer = std::make_unique<SimpleRenderer>(
        displaySpec,
        mapper,
        runningRendererName
    );
    blendingRenderer = std::make_unique<SimpleRenderer>(
        displaySpec,
        mapper,
        blendingRendererName
    );

    transitionPixelMapper = std::make_shared<SimplePixelMapper>(displaySpec);
}

void Blender::changeEffect(
    std::shared_ptr<Effect> effect,
    std::shared_ptr<Effect> overlay,
    std::shared_ptr<Effect> transition
) {
    currentTransition = transition;

    if (runningRenderer->getEffect() == nullptr) {
        runningRenderer->changeEffect(effect, overlay, transition);
    } else {
        transitionStep = transitionDurationInFrames;
        for (uint16_t pixelIndex = 0; pixelIndex < displaySpec.maxSegmentSize(); pixelIndex++) {
            transitionSegmentArray[pixelIndex] = 0;
        }
        blendingRenderer->changeEffect(effect, overlay, transition);
    }
}

void Blender::applyTransition(
    CRGB *outputArray,
    float transitionPercent
) const {
    auto context = currentTransition->effectContext;

    applyEffect(
        currentTransition,
        context.layoutIndex,
        context.mirror,
        transitionSegmentArray,
        transitionArray,
        transitionPercent,
        transitionPixelMapper
    );

    for (uint16_t pixelIndex = 0; pixelIndex < displaySpec.nbLeds(); pixelIndex++) {
        outputArray[pixelIndex] = blend(
            runningArray[pixelIndex],
            blendingArray[pixelIndex],
            transitionArray[pixelIndex].r
        );
    }
}

void Blender::render(CRGB *outputArray) {
    if (transitionStep < 0) {
        runningRenderer->render(outputArray);
    } else {
        runningRenderer->render(outputArray);
        blendingRenderer->render(outputArray);

        const float transitionPercent = 1 - (transitionStep / static_cast<float>(transitionDurationInFrames));
        const float smoothed = 1.0f - cos(transitionPercent * PI / 2); //sine easing

        applyTransition(outputArray, smoothed);
        transitionStep = max(-1, transitionStep - 1);

        if (transitionStep < 0) {
            runningRenderer->changeEffect(
                blendingRenderer->getEffect(),
                blendingRenderer->getOverlay(),
                currentTransition
            );
        }
    }
}

void Blender::mapPixels(
    const String &rendererName,
    const uint16_t layoutIndex,
    const uint16_t segmentIndex,
    const uint16_t segmentSize,
    float progress,
    CRGB *outputArray,
    CRGB *segmentArray
) {
    if (segmentArray == nullptr) {
        if constexpr (IS_DEBUG) Serial.println("Effect array is null for renderer " + rendererName);
        return; //should not happen
    }

    bool isRunningRenderer = rendererName == runningRendererName;

    CRGB *rendererOutputArray = transitionStep < 0
                                    ? outputArray
                                    : isRunningRenderer
                                          ? runningArray
                                          : blendingArray;

    for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
        displaySpec.setColour(
            layoutIndex,
            segmentIndex,
            pixelIndex,
            progress,
            rendererOutputArray,
            segmentArray[pixelIndex]
        );
    }
}

std::shared_ptr<Effect> Blender::getEffect() {
    return nullptr; //NOOP
}

std::shared_ptr<Effect> Blender::getOverlay() {
    return nullptr; //NOOP
}
