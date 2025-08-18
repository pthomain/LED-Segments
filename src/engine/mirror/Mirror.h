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

#ifndef LED_SEGMENTS_MIRROR_H
#define LED_SEGMENTS_MIRROR_H

#include "FastLED.h"
#include "engine/render/renderable/RenderableOperation.h"
#include <memory>

namespace LEDSegments {

template <typename C> class Renderable;

enum class Mirror {
    // The following mirrors apply to all effect types (effects, overlays and transitions).
    NONE,
    REVERSE,
    CENTRE,
    EDGE,
    REPEAT,
    REPEAT_REVERSE,

    // Below are mirrors for overlays and transitions only.

    // OVERLAY_REPEAT_X will repeat the overlay by dividing the entire segment in X parts
    // to define offsets and then repeat the same overlay X times across the entire segment.
    // This might cause interferences which are useful for Moiré patterns on circular displays.
    // It's better to only use multiples of your number of segments for even spacing.

    OVERLAY_REPEAT_2,
    OVERLAY_REPEAT_3,
    OVERLAY_REPEAT_4,
    OVERLAY_REPEAT_5,
    OVERLAY_REPEAT_6,

    // OVERLAY_REVERSE will add a mirror image of the overlay with an offset of half the segment size (with wrapping).
    OVERLAY_REVERSE,

    // OVERLAY_REPEAT_X_REVERSE will first repeat the overlay X times as described with OVERLAY_REPEAT_X,
    // then will mirror all those repetitions with an offset of the segment size divided by X (with wrapping).

    OVERLAY_REPEAT_2_REVERSE,
    OVERLAY_REPEAT_3_REVERSE,
    OVERLAY_REPEAT_4_REVERSE,
    OVERLAY_REPEAT_5_REVERSE,
    OVERLAY_REPEAT_6_REVERSE,
};

String getMirrorName(Mirror mirror);

uint16_t getMirrorSize(
    Mirror mirror,
    uint16_t segmentSize
);

template<typename C>
void applyMirror(
    const std::shared_ptr<Renderable<C> > &renderable,
    Mirror mirror,
    C *segmentArray,
    uint16_t segmentSize
);

RenderableOperation mixOperation(RenderableOperation operation);

} // namespace LEDSegments

#endif //LED_SEGMENTS_MIRROR_H
