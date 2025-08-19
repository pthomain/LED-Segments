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

#include "SwirlEffect.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

const uint8_t SwirlEffect::PARAM_IS_REVERSIBLE;

static const SwirlEffectFactory factoryInstance;
RenderableFactoryRef<CRGB> SwirlEffect::factory = &factoryInstance;

void SwirlEffect::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentSize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
   const int8_t direction = isReversed ? -1 : 1;
   const uint8_t step = segmentIndex * max(1, 255 / context.nbSegments);

   const auto offset = max(1, static_cast<uint8_t>(timeElapsedInMillis / speedDivider));
   const uint8_t start = direction * (step + offset);

    fill_palette(
        segmentArray,
        segmentSize,
        start,
        max(1, 255 / segmentSize),
        context.palette.palette,
        255,
        LINEARBLEND
    );
}

} // namespace LEDSegments
