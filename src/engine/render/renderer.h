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

#include "PixelMapper.h"
#include "engine/effect/Effect.h"
#include "engine/displayspec/DisplaySpec.h"

class Renderer {
protected:
    const DisplaySpec &displaySpec;
    const String name;

    void applyEffect(
        const std::shared_ptr<Effect> &effect,
        uint16_t layoutIndex,
        Mirror mirror,
        CRGB *segmentArray,
        CRGB *outputArray,
        float progress,
        const std::shared_ptr<PixelMapper> &pixelMapper
    ) const;

public:
    explicit Renderer(
        const DisplaySpec &displaySpec,
        String name
    ) : displaySpec(displaySpec), name(std::move(name)) {
    }

    virtual void changeEffect(
        std::shared_ptr<Effect> effect,
        std::shared_ptr<Effect> overlay,
        std::shared_ptr<Effect> transition
    ) = 0;

    virtual void render(CRGB *outputArray) = 0;

    virtual std::shared_ptr<Effect> getEffect() = 0;

    virtual std::shared_ptr<Effect> getOverlay() = 0;

    virtual ~Renderer() = default;
};

#endif //LED_SEGMENTS_RENDERER_H
