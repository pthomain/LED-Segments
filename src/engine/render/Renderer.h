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

    CRGB *outputArray = nullptr;
    CRGB *segmentArray = nullptr;
    uint8_t *segmentArray8 = nullptr;
    CRGB *pendingOutputArray = nullptr;

    std::shared_ptr<Effect<CRGB> > effect = nullptr;
    std::shared_ptr<Effect<CRGB> > overlay = nullptr;
    std::shared_ptr<Effect<uint8_t> > transition = nullptr;

    std::shared_ptr<Effect<CRGB> > pendingEffect = nullptr;
    std::shared_ptr<Effect<CRGB> > pendingOverlay = nullptr;
    std::shared_ptr<Effect<uint8_t> > pendingTransition = nullptr;

    float effectFrameIndex = 0.0f;
    float pendingEffectFrameIndex = 0.0f;
    float transitionStep = -1.0f;
    float transitionDurationInFrames = -1.0f;

    static bool validateEffect(
        const std::shared_ptr<Effect<CRGB> > &effect,
        const std::shared_ptr<Effect<CRGB> > &overlay,
        const std::shared_ptr<Effect<uint8_t> > &transition
    );

    template<typename C>
    void applyEffectOrTransition(
        const std::shared_ptr<Effect<C> > &effect,
        C *segmentArray,
        CRGB *outputArray,
        std::function<CRGB(uint16_t ledIndex, CRGB existing, C toBeMixed)> mix,
        float progress
    ) const;

    void flattenEffectAndOverlay(
        const std::shared_ptr<Effect<CRGB> > &effect,
        const std::shared_ptr<Effect<CRGB> > &overlay,
        float progress,
        CRGB *outputArray
    ) const;

public:
    explicit Renderer(const DisplaySpec &displaySpec, CRGB *outputArray);

    void changeEffect(
        const std::shared_ptr<Effect<CRGB> > &effect,
        const std::shared_ptr<Effect<CRGB> > &overlay,
        const std::shared_ptr<Effect<uint8_t> > &transition
    );

    void render();

    ~Renderer();
};

#endif //LED_SEGMENTS_RENDERER_H
