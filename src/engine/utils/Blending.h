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

#pragma once

#ifndef BLENDING_H
#define BLENDING_H

#include "FastLED.h"
#include "engine/render/renderable/RenderableOperation.h"

namespace LEDSegments {
uint8_t multiply(uint8_t base, uint8_t overlay);

CRGB multiply(const CRGB &base, const CRGB &overlay);

uint8_t screen(uint8_t base, uint8_t overlay);

CRGB screen(const CRGB &base, const CRGB &overlay);

uint8_t invert(uint8_t base, uint8_t overlay);

CRGB invert(const CRGB &base, const CRGB &overlay);

template<typename T>
T mix(const T &base, const T &overlay, RenderableOperation operation) {
    switch (operation) {
        case RenderableOperation::OVERLAY_SCREEN:
            return screen(base, overlay);

        case RenderableOperation::OVERLAY_MULTIPLY:
            return multiply(base, overlay);

        case RenderableOperation::OVERLAY_INVERT:
            return invert(base, overlay);

        default:
            return overlay;
    }
}

template
CRGB mix(const CRGB &base, const CRGB &overlay, RenderableOperation operation);

template
uint8_t mix(const uint8_t &base, const uint8_t &overlay, RenderableOperation operation);

} // namespace LEDSegments

#endif //BLENDING_H
