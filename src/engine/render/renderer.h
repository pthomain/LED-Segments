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

#ifndef LED_SEGMENTS_RENDERER_H
#define LED_SEGMENTS_RENDERER_H

#include "engine/effect/Effect.h"
#include "engine/displayspec/DisplaySpec.h"

class Renderer {
protected:
    const DisplaySpec &displaySpec;
    const String name;

public:
    explicit Renderer(
        const DisplaySpec &displaySpec,
        String name
    ) : displaySpec(displaySpec), name(std::move(name)) {
    }

    virtual void changeEffect(std::shared_ptr<Effect> effect) = 0;

    virtual void render(CRGB *outputArray) = 0;

    virtual std::shared_ptr<Effect> getEffect() = 0;

    virtual ~Renderer() = default;
};


#endif //LED_SEGMENTS_RENDERER_H
