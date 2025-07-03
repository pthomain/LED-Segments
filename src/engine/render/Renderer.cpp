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
#include "engine/effect/BaseEffect.h"
#include "engine/utils/Utils.h"

Renderer::Renderer(
    const std::shared_ptr<DisplaySpec> &displaySpec,
    CRGB *outputArray
) : displaySpec(displaySpec),
    outputArray(outputArray) {
    segmentArray = new CRGB[displaySpec->maxSegmentSize()]();
    segmentArray8 = new uint8_t[displaySpec->maxSegmentSize()]();
    pendingOutputArray = new CRGB[displaySpec->nbLeds()]();
}

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<BaseEffect<CRGB> > &effect,
    CRGB *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, CRGB toBeMixed)> mix,
    float progress
) const;

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<BaseEffect<uint8_t> > &effect,
    uint8_t *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, uint8_t toBeMixed)> mix,
    float progress
) const;

template<typename C>
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<BaseEffect<C> > &effect,
    C *segmentArray,
    CRGB *outputArray,
    std::function<CRGB(uint16_t ledIndex, CRGB existing, C toBeMixed)> mix,
    float progress
) const {
    auto context = effect->context;
    auto layoutId = context.layoutId;
    auto mirror = context.mirror;

    auto nbSegments = displaySpec->nbSegments(layoutId);

    for (auto segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        auto segmentSize = displaySpec->segmentSize(layoutId, segmentIndex);
        uint16_t mirrorSize = getMirrorSize(mirror, segmentSize);

        effect->fillArray(
            segmentArray,
            mirrorSize,
            segmentIndex,
            progress
        );

        applyMirror(mirror, segmentArray, segmentSize);

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
                        Serial.print("Invalid LED index: ");
                        Serial.println(ledIndex);
                    }
                }
            );
        }
    }
}

Renderer::~Renderer() {
    delete[] segmentArray;
    delete[] segmentArray8;
    delete[] pendingOutputArray;
}

bool Renderer::validateEffect(
    const std::shared_ptr<BaseEffect<CRGB> > &effect,
    const std::shared_ptr<BaseEffect<CRGB> > &overlay,
    const std::shared_ptr<BaseEffect<uint8_t> > &transition
) {
    return effect && overlay && transition
           && effect->effectOperation() == EffectOperation::EFFECT
           && (
               overlay->effectOperation() == EffectOperation::OVERLAY_SOURCE
               || overlay->effectOperation() == EffectOperation::OVERLAY_SCREEN
               || overlay->effectOperation() == EffectOperation::OVERLAY_MULTIPLY
               || overlay->effectOperation() == EffectOperation::OVERLAY_INVERT
           )
           && transition->effectOperation() == EffectOperation::TRANSITION;
}

void Renderer::changeEffect(
    const std::shared_ptr<BaseEffect<CRGB> > &effect,
    const std::shared_ptr<BaseEffect<CRGB> > &overlay,
    const std::shared_ptr<BaseEffect<uint8_t> > &transition
) {
    if (!validateEffect(effect, overlay, transition)) return;

    if (!validateEffect(this->effect, this->overlay, this->transition)) {
        this->effect = effect;
        this->overlay = overlay;
        this->transition = transition;
    } else {
        //TODO handle case when effect is changed during transition
        transitionDurationInFrames = static_cast<float>(transition->context.durationInFrames);
        transitionStep = transitionDurationInFrames;
        pendingEffectFrameIndex = 0;

        this->pendingEffect = effect;
        this->pendingOverlay = overlay;
        this->pendingTransition = transition;
    }
}

void Renderer::render() {
    if (!validateEffect(effect, overlay, transition)) {
        Serial.println("Could not render");
        return;
    }

    //Render main effect in outputArray
    float effectDurationInFrames = static_cast<float>(effect->context.durationInFrames) + transitionDurationInFrames;
    float effectProgress = min(1.0f, effectFrameIndex / effectDurationInFrames);
    effectFrameIndex++;

    flattenEffectAndOverlay(effect, overlay, effectProgress, outputArray);

    //Return if no transition is running
    if (transitionStep <= 0) return;

    //Render pending effect in pendingOutputArray if transition is running
    float pendingEffectDurationInFrames = static_cast<float>(pendingEffect->context.durationInFrames) +
                                          transitionDurationInFrames;
    float pendingEffectProgress = min(1.0f, pendingEffectFrameIndex / pendingEffectDurationInFrames);
    pendingEffectFrameIndex++;

    flattenEffectAndOverlay(pendingEffect, pendingOverlay, pendingEffectProgress, pendingOutputArray);

    //Render transition in transitionOutputArray
    const float transitionPercent = 1.0f - (transitionStep / transitionDurationInFrames);
    const float smoothedTransition = 1.0f - static_cast<float>(cos(transitionPercent * PI / 2.0f)); //sine easing

    applyEffectOrTransition<uint8_t>(
        transition,
        segmentArray8,
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
    const std::shared_ptr<BaseEffect<CRGB> > &effect,
    const std::shared_ptr<BaseEffect<CRGB> > &overlay,
    float progress,
    CRGB *effectOutputArray
) const {
    applyEffectOrTransition<CRGB>(
        effect,
        segmentArray,
        effectOutputArray,
        [](uint16_t, CRGB, CRGB toBeMixed) { return toBeMixed; },
        progress
    );

    applyEffectOrTransition<CRGB>(
        overlay,
        segmentArray,
        effectOutputArray,
        [&](uint16_t, CRGB existing, CRGB toBeMixed) {
            switch (overlay->effectOperation()) {
                case EffectOperation::OVERLAY_SCREEN: return screen(existing, toBeMixed);
                case EffectOperation::OVERLAY_MULTIPLY: return multiply(existing, toBeMixed);
                default: return existing; //TODO
            }
        },
        progress
    );
}
