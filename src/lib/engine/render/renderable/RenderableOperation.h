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

#ifndef RENDERABLE_TYPE_H
#define RENDERABLE_TYPE_H

enum class RenderableType {
    EFFECT,
    OVERLAY,
    TRANSITION
};

enum class RenderableOperation {
    EFFECT,             //default for effects, always uses source (CRGB)
    OVERLAY_SOURCE,     //only useful for Overlay::NONE (CRGB)
    OVERLAY_SCREEN,     //blends using screen mode (CRGB)
    OVERLAY_MULTIPLY,   //blends using multiply mode (CRGB grayscale) //TODO enforce
    OVERLAY_INVERT,     //inverts colours by 180 degrees (boolean: CRGB::Black = false, any other value = true)
    TRANSITION          //default for transitions, always uses multiply (uint8_t)
};

inline String renderableTypeName(RenderableType type) {
    switch (type) {
        case RenderableType::EFFECT:
            return "EFFECT";
        case RenderableType::OVERLAY:
            return "OVERLAY";
        case RenderableType::TRANSITION:
            return "TRANSITION";
        default:
            return "UNKNOWN";
    }
}

#endif //RENDERABLE_TYPE_H
