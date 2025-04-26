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

#include "SimpleRenderer.h"
#include "engine/render/PixelMapper.h"

SimpleRenderer::SimpleRenderer(
    const DisplaySpec &displaySpec,
    std::unique_ptr<PixelMapper> pixelMapper,
    const String &name
) : Renderer(displaySpec, name),
    effectArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    pixelMapper(std::move(pixelMapper)) {
}

void SimpleRenderer::changeEffect(std::shared_ptr<Effect> effect) {
    currentEffect = effect;
    frameIndex = 0;
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        if constexpr (IS_DEBUG) Serial.println("No effect on " + name);
        return;
    }

    EffectContext context = currentEffect->effectContext;
    uint16_t layoutIndex = context.layoutIndex;
    uint16_t nbSegments = displaySpec.nbSegments(layoutIndex);

    for (uint8_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
        uint16_t nbPixels = displaySpec.nbPixels(layoutIndex, segmentIndex);
        uint16_t mirrorSize = getMirrorSize(context.mirror, nbPixels);

        currentEffect->fillArray(effectArray, mirrorSize, segmentIndex, frameIndex);
        applyMirror(context.mirror, effectArray, nbPixels);

        pixelMapper->mapPixels(
            name,
            layoutIndex,
            segmentIndex,
            nbPixels,
            frameIndex,
            outputArray,
            effectArray
        );
    }

    frameIndex++;
}

std::shared_ptr<Effect> SimpleRenderer::getEffect() {
    return currentEffect;
}
