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

#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "FastLED.h"
#include "engine/effect/Effect.h"
#include "engine/displayspec/DisplaySpec.h"

class Renderer {
    const DisplaySpec &displaySpec;

    CRGB *segmentArray = nullptr;
    CRGB *bufferOutputArray = nullptr;
    CRGB *pendingOutputArray = nullptr;
    CRGB *transitionOutputArray = nullptr; //TODO use uint8_t

    std::shared_ptr<Effect> effect = nullptr;
    std::shared_ptr<Effect> overlay = nullptr;
    std::shared_ptr<Effect> transition = nullptr;

    std::shared_ptr<Effect> pendingEffect = nullptr;
    std::shared_ptr<Effect> pendingOverlay = nullptr;
    std::shared_ptr<Effect> pendingTransition = nullptr;

    float effectFrameIndex = 0.0f;
    float pendingEffectFrameIndex = 0.0f;
    float transitionStep = -1.0f;
    float transitionDurationInFrames = -1.0f;

    static bool validateEffect(
        const std::shared_ptr<Effect> &effect,
        const std::shared_ptr<Effect> &overlay,
        const std::shared_ptr<Effect> &transition
    );

    void applyEffect(
        const std::shared_ptr<Effect> &effect,
        CRGB *outputArray,
        float progress
    ) const;

    void flattenEffectAndOverlay(
        const std::shared_ptr<Effect> &effect,
        const std::shared_ptr<Effect> &overlay,
        float progress,
        CRGB *outputArray
    ) const;

public:
    explicit Renderer(const DisplaySpec &displaySpec);

    void changeEffect(
        const std::shared_ptr<Effect> &effect,
        const std::shared_ptr<Effect> &overlay,
        const std::shared_ptr<Effect> &transition
    );

    void render(CRGB *outputArray);

    ~Renderer();
};

#endif //LED_SEGMENTS_RENDERER_H
