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

#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "engine/render/Renderer.h"
#include "engine/render/PixelMapper.h"

class SimpleRenderer : public Renderer {
    CRGB *effectSegmentArray;
    CRGB *overlaySegmentArray;
    CRGB *overlayBlendingArray;
    std::shared_ptr<Effect> currentEffect = nullptr;
    std::shared_ptr<Effect> currentOverlay = nullptr;
    std::shared_ptr<PixelMapper> simplePixelMapper = nullptr;
    std::shared_ptr<PixelMapper> callbackPixelMapper = nullptr;
    float frameIndex = 0;

public :
    explicit SimpleRenderer(
        const DisplaySpec &displaySpec,
        std::shared_ptr<PixelMapper> callbackPixelMapper,
        const String &name
    );

    void changeEffect(
        std::shared_ptr<Effect> effect,
        std::shared_ptr<Effect> overlay,
        std::shared_ptr<Effect> transition
    ) override;

    void render(CRGB *outputArray) override;

    std::shared_ptr<Effect> getEffect() override;

    std::shared_ptr<Effect> getOverlay() override;

    ~SimpleRenderer() override {
        delete[] effectSegmentArray;
        delete[] overlaySegmentArray;
        delete[] overlayBlendingArray;
        effectSegmentArray = nullptr;
        overlaySegmentArray = nullptr;
        overlayBlendingArray = nullptr;
        if (simplePixelMapper) simplePixelMapper.reset();
        if (callbackPixelMapper) callbackPixelMapper.reset();
        if (currentEffect) currentEffect.reset();
        if (currentOverlay) currentOverlay.reset();
    }
};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
