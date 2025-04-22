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

#ifndef LED_SEGMENTS_BLENDER_H
#define LED_SEGMENTS_BLENDER_H

#include "FastLED.h"
#include "engine/effect/Effect.h"
#include "engine/render/Renderer.h"
#include "engine/render/PixelMapper.h"
#include "engine/displayspec/DisplaySpec.h"
#include "memory"

class Blender : public Renderer, PixelMapper {
    std::unique_ptr<Renderer> runningRenderer = nullptr;
    std::unique_ptr<Renderer> blendingRenderer = nullptr;

    CRGB *runningArray;
    CRGB *blendingArray;

    CRGB *transitionSegmentArray;
    CRGB *transitionArray;
    float transitionStep = -1;

    //todo shared pointer
    const EffectContext *currentEffectContext = nullptr;

    const String runningRendererName = "runningRenderer";
    const String blendingRendererName = "blendingRenderer";

    const uint16_t refreshRateInMillis;
    const uint16_t transitionDurationInMillis;
    const uint16_t transitionDurationInFrames = transitionDurationInMillis / refreshRateInMillis;

    void fillTransition(float transitionPercent) const;

    void applyTransition(CRGB *outputArray, float transitionPercent) const;

public :
    explicit Blender(
        const DisplaySpec &displaySpec,
        const String &name,
        const uint16_t refreshRateInMillis,
        const uint16_t transitionDurationInMillis
    );

    void changeEffect(std::shared_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    std::shared_ptr<Effect> getEffect() override;

    void mapPixels(
        const String &rendererName,
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t segmentSize,
        const uint16_t frameIndex,
        CRGB *outputArray,
        CRGB *effectArray
    ) override;

    ~Blender() override {
        delete[] runningArray;
        delete[] blendingArray;
        delete[] transitionSegmentArray;
        delete[] transitionArray;
        if (runningRenderer) runningRenderer.reset();
        if (blendingRenderer) blendingRenderer.reset();
    }
};

#endif //LED_SEGMENTS_BLENDER_H
