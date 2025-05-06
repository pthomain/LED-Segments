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

#include "Renderer.h"

#include "PixelMapper.h"

void Renderer::applyEffect(
    const std::shared_ptr<Effect> &effect,
    uint16_t layoutIndex,
    Mirror mirror,
    CRGB *segmentArray,
    CRGB *outputArray,
    float progress,
    const std::shared_ptr<PixelMapper> &pixelMapper
) const {
    auto nbSegments = displaySpec.nbSegments(layoutIndex);

    for (auto segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        auto segmentSize = displaySpec.nbPixels(layoutIndex, segmentIndex);
        uint16_t mirrorSize = getMirrorSize(mirror, segmentSize);

        effect->fillArray(
            segmentArray,
            mirrorSize,
            progress
        );

        applyMirror(mirror, segmentArray, segmentSize);

        pixelMapper->mapPixels(
            name,
            layoutIndex,
            segmentIndex,
            segmentSize,
            progress,
            outputArray,
            segmentArray
        );
    }
}
