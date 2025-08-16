//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain

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

#include "Renderer.h"
#include "engine/utils/Blending.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Interpolation.h"

Renderer::Renderer(
    const std::shared_ptr<DisplaySpec> &displaySpec,
    CRGB *outputArray
) : displaySpec(displaySpec),
    outputArray(outputArray) {
    renderableArray = new CRGB[displaySpec->maxSegmentSize()]();
    renderableArray8 = new uint8_t[displaySpec->maxSegmentSize()]();
    pendingOutputArray = new CRGB[displaySpec->nbLeds()]();
}

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<CRGB> > &renderable,
    CRGB *renderableArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, CRGB toBeMixed)> mix,
    float progress
) const;

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<uint8_t> > &renderable,
    uint8_t *renderableArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, uint8_t toBeMixed)> mix,
    float progress
) const;

template<typename C>
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<C> > &renderable,
    C *renderableArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, C toBeMixed)> mix,
    float progress
) const {
    auto context = renderable->context;
    auto layoutId = context.layoutId;
    auto mirror = context.mirror;

    auto nbSegments = displaySpec->nbSegments(layoutId);

    for (auto segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        auto segmentSize = displaySpec->segmentSize(layoutId, segmentIndex);
        if (segmentSize == 0) {
            Serial.print("No pixels for layout ");
            Serial.print(displaySpec->config.layoutName(layoutId));
            Serial.print(" at segmentIndex ");
            Serial.println(segmentIndex);
            continue;
        }

        uint16_t mirrorSize = getMirrorSize(mirror, segmentSize);

        renderable->fillArray(
            renderableArray,
            mirrorSize,
            segmentIndex,
            progress
        );

        applyMirror(renderable, mirror, renderableArray, segmentSize);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec->mapLeds(
                layoutId,
                segmentIndex,
                pixelIndex,
                progress,
                [&](uint16_t ledIndex) {
                    if (ledIndex < displaySpec->nbLeds()) {
                        outputArray[ledIndex] = mix(ledIndex, outputArray[ledIndex], renderableArray[pixelIndex]);
                    } else {
                        Serial.print("Invalid LED index: ");
                        Serial.println(ledIndex);
                    }
                }
            );
        }
    }
}

Renderer::~Renderer() {
    delete[] renderableArray;
    delete[] renderableArray8;
    delete[] pendingOutputArray;
}

bool Renderer::validateRenderables(
    const std::shared_ptr<Renderable<CRGB> > &effect,
    const std::shared_ptr<Renderable<CRGB> > &overlay,
    const std::shared_ptr<Renderable<uint8_t> > &transition
) {
    return effect && overlay && transition
           && effect->renderableOperation() == RenderableOperation::EFFECT
           && (
               overlay->renderableOperation() == RenderableOperation::OVERLAY_SOURCE
               || overlay->renderableOperation() == RenderableOperation::OVERLAY_SCREEN
               || overlay->renderableOperation() == RenderableOperation::OVERLAY_MULTIPLY
               || overlay->renderableOperation() == RenderableOperation::OVERLAY_INVERT
           )
           && transition->renderableOperation() == RenderableOperation::TRANSITION;
}

void Renderer::changeEffect(
    const std::shared_ptr<Renderable<CRGB> > &effect,
    const std::shared_ptr<Renderable<CRGB> > &overlay,
    const std::shared_ptr<Renderable<uint8_t> > &transition
) {
    if (!validateRenderables(effect, overlay, transition)) return;

    if (!validateRenderables(this->effect, this->overlay, this->transition)) {
        this->effect = effect;
        this->overlay = overlay;
        this->transition = transition;
    } else {
        transitionDurationInFrames = static_cast<float>(transition->context.durationInFrames);
        transitionStep = transitionDurationInFrames;
        pendingEffectFrameIndex = 0;

        this->pendingEffect = effect;
        this->pendingOverlay = overlay;
        this->pendingTransition = transition;
    }
}

void Renderer::render() {
    if (!validateRenderables(effect, overlay, transition)) {
        Serial.println("Could not render");
        return;
    }

    //Render main effect in outputArray
    float effectDurationInFrames = static_cast<float>(effect->context.durationInFrames) + transitionDurationInFrames;
    float effectProgress = min(1.0f, effectFrameIndex / effectDurationInFrames);
    effectFrameIndex++;

    flattenEffectAndOverlay(
        effect,
        overlay,
        effectProgress,
        outputArray
    );

    //Return if no transition is running
    if (transitionStep <= 0) return;

    //Render pending effect in pendingOutputArray if transition is running
    float pendingEffectDurationInFrames = static_cast<float>(pendingEffect->context.durationInFrames) +
                                          transitionDurationInFrames;
    float pendingEffectProgress = min(1.0f, pendingEffectFrameIndex / pendingEffectDurationInFrames);
    pendingEffectFrameIndex++;

    flattenEffectAndOverlay(
        pendingEffect,
        pendingOverlay,
        pendingEffectProgress,
        pendingOutputArray
    );

    //Render transition in transitionOutputArray
    const float transitionPercent = 1.0f - (transitionStep / transitionDurationInFrames);
    const float smoothedTransition = Interpolation::easeInOutQuad(transitionPercent);

    applyEffectOrTransition<uint8_t>(
        transition,
        renderableArray8,
        outputArray,
        [&](uint16_t ledIndex, CRGB _, uint8_t toBeMixed) {
            return blend(
                outputArray[ledIndex],
                pendingOutputArray[ledIndex],
                toBeMixed
            );
        },
        smoothedTransition
    );

    transitionStep--;

    if (transitionStep <= 0) {
        effect = std::move(pendingEffect);
        overlay = std::move(pendingOverlay);
        transition = std::move(pendingTransition);
        effectFrameIndex = pendingEffectFrameIndex; //TODO check transition duration impact on this
        pendingEffectFrameIndex = 0;
    }
}

void Renderer::flattenEffectAndOverlay(
    const std::shared_ptr<Renderable<CRGB> > &effect,
    const std::shared_ptr<Renderable<CRGB> > &overlay,
    float progress,
    CRGB *effectOutputArray
) const {
    applyEffectOrTransition<CRGB>(
        effect,
        renderableArray,
        effectOutputArray,
        [](uint16_t, CRGB, CRGB toBeMixed) { return toBeMixed; },
        progress
    );

    applyEffectOrTransition<CRGB>(
        overlay,
        renderableArray,
        effectOutputArray,
        [&](uint16_t, CRGB existing, CRGB toBeMixed) {
            return mix(
                existing,
                toBeMixed,
                overlay->renderableOperation()
            );
        },
        progress
    );
}
