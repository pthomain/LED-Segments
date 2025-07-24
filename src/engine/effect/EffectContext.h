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

#ifndef LED_SEGMENTS_EFFECTCONTEXT_H
#define LED_SEGMENTS_EFFECTCONTEXT_H

#include <map>
#include <engine/mirror/Mirror.h>
#include <engine/palette/Palette.h>

class EffectContext {
public:
    const uint16_t maxSegmentSize;
    const uint16_t nbSegments;
    const uint16_t durationInFrames;
    const boolean isDisplayCircular;
    const uint16_t layoutId;
    const Palette palette;
    const Mirror mirror;
    const std::map<uint8_t, uint16_t> parameters;

    EffectContext(
        const uint16_t maxSegmentSize,
        const uint16_t nbSegments,
        const uint16_t durationInFrames,
        const boolean isDisplayCircular,
        const uint16_t layoutId,
        Palette palette,
        const Mirror mirror,
        const std::map<uint8_t, uint16_t> &parameters
    ) : maxSegmentSize(maxSegmentSize),
        nbSegments(nbSegments),
        durationInFrames(durationInFrames),
        isDisplayCircular(isDisplayCircular),
        layoutId(layoutId),
        palette(std::move(palette)),
        mirror(mirror),
        parameters(std::move(parameters)) {
    }
};

#endif //LED_SEGMENTS_EFFECTCONTEXT_H
