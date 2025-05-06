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

#include "SimpleRenderer.h"
#include "SimplePixelMapper.h"
#include "engine/render/PixelMapper.h"

SimpleRenderer::SimpleRenderer(
    const DisplaySpec &displaySpec,
    std::shared_ptr<PixelMapper> callbackPixelMapper,
    const String &name
) : Renderer(displaySpec, name),
    effectSegmentArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    overlaySegmentArray(new CRGB[displaySpec.maxSegmentSize()]{}),
    overlayBlendingArray(new CRGB[displaySpec.nbLeds()]{}),
    simplePixelMapper(std::make_shared<SimplePixelMapper>(displaySpec)),
    callbackPixelMapper(std::move(callbackPixelMapper)) {
}

void SimpleRenderer::changeEffect(
    std::shared_ptr<Effect> effect,
    std::shared_ptr<Effect> overlay,
    std::shared_ptr<Effect> transition
) {
    currentEffect = effect;
    currentOverlay = overlay;
    frameIndex = 0;
}

void SimpleRenderer::render(CRGB *outputArray) {
    if (currentEffect == nullptr) {
        if constexpr (IS_DEBUG) Serial.println("No effect on " + name);
        return;
    }

    EffectContext effectContext = currentEffect->effectContext;
    EffectContext overlayContext = currentOverlay->effectContext;
    float progress = frameIndex / static_cast<float>(effectContext.durationsInFrames);
    frameIndex++;

    applyEffect(
        currentEffect,
        effectContext.layoutIndex,
        effectContext.mirror,
        effectSegmentArray,
        outputArray,
        progress,
        callbackPixelMapper
    );

    // applyEffect(
    //     currentOverlay,
    //     overlayContext.layoutIndex,
    //     overlayContext.mirror,
    //     overlaySegmentArray,
    //     overlayBlendingArray,
    //     progress,
    //     thisPixelMapper
    // );
    //
    // if (currentOverlay->type() == EffectType::OVERLAY_COLOUR) {
    //     for (uint16_t ledIndex = 0; ledIndex < displaySpec.nbLeds(); ledIndex++) {
    //         if (overlayBlendingArray[ledIndex] != CRGB::Black)
    //             outputArray[ledIndex] = overlayBlendingArray[ledIndex];
    //     }
    // }
}

std::shared_ptr<Effect> SimpleRenderer::getEffect() {
    return currentEffect;
}

std::shared_ptr<Effect> SimpleRenderer::getOverlay() {
    return currentOverlay;
}
