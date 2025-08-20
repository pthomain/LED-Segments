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
#include "engine/utils/Interpolation.h"
#include "engine/utils/Utils.h"
#include <memory>

namespace LEDSegments {

Renderer::Renderer(
    const std::shared_ptr<DisplaySpec> &displaySpec,
    CRGB *outputArray
) : displaySpec(displaySpec),
    outputArray(outputArray),
    segmentArray(std::make_unique<CRGB[]>(displaySpec->maxSegmentSize())),
    segmentArray8(std::make_unique<uint8_t[]>(displaySpec->maxSegmentSize())),
    pendingOutputArray(std::make_unique<CRGB[]>(displaySpec->nbLeds())) {
    // Initialize arrays to zero
    std::fill_n(segmentArray.get(), displaySpec->maxSegmentSize(), CRGB::Black);
    std::fill_n(segmentArray8.get(), displaySpec->maxSegmentSize(), 0);
    std::fill_n(pendingOutputArray.get(), displaySpec->nbLeds(), CRGB::Black);
}

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<CRGB> > &renderable,
    CRGB *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, CRGB toBeMixed)> mix,
    fract16 progress
) const;

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<uint8_t> > &renderable,
    uint8_t *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, uint8_t toBeMixed)> mix,
    fract16 progress
) const;

template<typename C>
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Renderable<C> > &renderable,
    C *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, C toBeMixed)> mix,
    fract16 progress
) const {
    auto context = renderable->context;
    auto layoutId = context.layoutId;
    auto mirror = context.mirror;

    auto nbSegments = displaySpec->nbSegments(layoutId);

    for (auto segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        auto segmentSize = displaySpec->segmentSize(layoutId, segmentIndex);
        if (segmentSize == 0) {
            #ifdef DEBUG
            Serial.print("No pixels for layout ");
            Serial.print(displaySpec->config.layoutName(layoutId));
            Serial.print(" at segmentIndex ");
            Serial.println(segmentIndex);
            #endif
            continue;
        }

        uint16_t mirrorSize = getMirrorSize(mirror, segmentSize);

        renderable->fillArray(
            segmentArray,
            mirrorSize,
            segmentIndex,
            progress
        );

        applyMirror(renderable, mirror, segmentArray, segmentSize);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec->mapLeds(
                layoutId,
                segmentIndex,
                pixelIndex,
                progress,
                [&](uint16_t ledIndex) {
                    if (ledIndex < displaySpec->nbLeds()) {
                        outputArray[ledIndex] = mix(ledIndex, outputArray[ledIndex], segmentArray[pixelIndex]);
                    } else {
                        #ifdef DEBUG
                        Serial.print("Invalid LED index: ");
                        Serial.println(ledIndex);
                        #endif
                    }
                }
            );
        }
    }
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
        #ifdef DEBUG
        Serial.println("Could not render");
        #endif
        return;
    }

    //Render main effect in outputArray
    float effectDurationInFrames = static_cast<float>(effect->context.durationInFrames) + transitionDurationInFrames;
    fract16 effectProgress = min(65535, (uint32_t)(effectFrameIndex * 65535 / effectDurationInFrames));
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
    fract16 pendingEffectProgress = min(65535, (uint32_t)(pendingEffectFrameIndex * 65535 / pendingEffectDurationInFrames));
    pendingEffectFrameIndex++;

    flattenEffectAndOverlay(
        pendingEffect,
        pendingOverlay,
        pendingEffectProgress,
        pendingOutputArray.get()
    );

    //Render transition in transitionOutputArray
    const fract16 transitionPercent = 65535 - (uint32_t)(transitionStep * 65535 / transitionDurationInFrames);
    const fract16 smoothedTransition = Interpolation::easeInOutQuad(transitionPercent);

    applyEffectOrTransition<uint8_t>(
        transition,
        segmentArray8.get(),
        outputArray,
        [&](uint16_t ledIndex, CRGB _, uint8_t toBeMixed) {
            return blend(
                outputArray[ledIndex],
                pendingOutputArray.get()[ledIndex],
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
        effectFrameIndex = pendingEffectFrameIndex;
        pendingEffectFrameIndex = 0;
    }
}

void Renderer::flattenEffectAndOverlay(
    const std::shared_ptr<Renderable<CRGB> > &effect,
    const std::shared_ptr<Renderable<CRGB> > &overlay,
    fract16 progress,
    CRGB *effectOutputArray
) const {
    applyEffectOrTransition<CRGB>(
        effect,
        segmentArray.get(),
        effectOutputArray,
        [](uint16_t, CRGB, CRGB toBeMixed) { return toBeMixed; },
        progress
    );

    applyEffectOrTransition<CRGB>(
        overlay,
        segmentArray.get(),
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

} // namespace LEDSegments
