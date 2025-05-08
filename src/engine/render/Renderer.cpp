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

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Effect<CRGB> > &effect,
    CRGB *segmentArray,
    CRGB *outputArray,
    float progress
) const;

template
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Effect<uint8_t> > &effect,
    uint8_t *segmentArray,
    uint8_t *outputArray,
    float progress
) const;

template<typename C>
void Renderer::applyEffectOrTransition(
    const std::shared_ptr<Effect<C> > &effect,
    C *segmentArray,
    C *outputArray,
    float progress
) const {
    auto context = effect->context;
    auto layoutIndex = context.layoutIndex;
    auto mirror = context.mirror;
    auto nbSegments = displaySpec.nbSegments(layoutIndex);

    for (auto segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        auto segmentSize = displaySpec.nbPixels(layoutIndex, segmentIndex);
        uint16_t mirrorSize = getMirrorSize(mirror, segmentSize);

        effect->fillArray(
            segmentArray,
            mirrorSize,
            progress
        );

        applyMirror(mirror, segmentArray, segmentSize);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentSize; pixelIndex++) {
            displaySpec.mapLeds(
                layoutIndex,
                segmentIndex,
                pixelIndex,
                progress,
                [&](uint16_t ledIndex) {
                    outputArray[ledIndex] = segmentArray[pixelIndex];
                }
            );
        }
    }
}

Renderer::Renderer(const DisplaySpec &displaySpec, CRGB *outputArray) : displaySpec(displaySpec),
                                                                        outputArray(outputArray) {
    segmentArray = new CRGB[displaySpec.maxSegmentSize()]();
    transitionSegmentArray = new uint8_t[displaySpec.maxSegmentSize()]();
    bufferOutputArray = new CRGB[displaySpec.nbLeds()]();
    pendingOutputArray = new CRGB[displaySpec.nbLeds()]();
    transitionOutputArray = new uint8_t[displaySpec.nbLeds()]();
}

Renderer::~Renderer() {
    delete[] outputArray;
    delete[] segmentArray;
    delete[] transitionSegmentArray;
    delete[] bufferOutputArray;
    delete[] pendingOutputArray;
    delete[] transitionOutputArray;
}

bool Renderer::validateEffect(
    const std::shared_ptr<Effect<CRGB> > &effect,
    const std::shared_ptr<Effect<CRGB> > &overlay,
    const std::shared_ptr<Effect<uint8_t> > &transition
) {
    return effect && overlay && transition
           && effect->type() == EffectType::EFFECT
           && (overlay->type() == EffectType::OVERLAY_ALPHA || overlay->type() == EffectType::OVERLAY_COLOUR)
           && transition->type() == EffectType::TRANSITION;
}

void Renderer::changeEffect(
    const std::shared_ptr<Effect<CRGB> > &effect,
    const std::shared_ptr<Effect<CRGB> > &overlay,
    const std::shared_ptr<Effect<uint8_t> > &transition
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

    applyEffectOrTransition(
        transition,
        transitionSegmentArray,
        transitionOutputArray,
        smoothedTransition
    );

    //Merge outputArray and pendingOutputArray using transition
    for (uint16_t ledIndex = 0; ledIndex < displaySpec.nbLeds(); ledIndex++) {
        outputArray[ledIndex] = blend(
            outputArray[ledIndex],
            pendingOutputArray[ledIndex],
            transitionOutputArray[ledIndex]
        );

        if (outputArray[ledIndex] == CRGB::Black) {
            Serial.print("empty outputArray ledIndex: ");
            Serial.println(ledIndex);
        } else if (pendingOutputArray[ledIndex] == CRGB::Black) {
            Serial.print("empty pendingOutputArray ledIndex: ");
            Serial.println(ledIndex);
        }
    }

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
    const std::shared_ptr<Effect<CRGB> > &effect,
    const std::shared_ptr<Effect<CRGB> > &overlay,
    float progress,
    CRGB *outputArray
) const {
    applyEffectOrTransition(
        effect,
        segmentArray,
        outputArray,
        progress
    );

    applyEffectOrTransition(
        overlay,
        segmentArray,
        bufferOutputArray,
        progress
    );

    //TODO fix this
    for (uint16_t ledIndex = 0; ledIndex < displaySpec.nbLeds(); ledIndex++) {
        if (overlay->type() == EffectType::OVERLAY_COLOUR) {
            if (bufferOutputArray[ledIndex] == CRGB::White)
                outputArray[ledIndex] = CRGB::White;
        } else if (overlay->type() == EffectType::OVERLAY_ALPHA) {
            // outputArray[ledIndex] = outputArray[ledIndex].nscale8_video(255 - bufferOutputArray[ledIndex].r);
        }
    }
}
