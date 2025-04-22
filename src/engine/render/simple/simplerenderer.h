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

#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "engine/render/Renderer.h"
#include "engine/render/PixelMapper.h"

class SimpleRenderer : public Renderer {
    CRGB *effectArray;
    std::shared_ptr<Effect> currentEffect = nullptr;
    uint16_t frameIndex = 0;

    //TODO unique ptr
    PixelMapper *pixelMapper;

public :
    explicit SimpleRenderer(
        const DisplaySpec &displaySpec,
        PixelMapper *pixelMapper,
        const String &name
    );

    // bool hasEffect() override {
    //     return currentEffect != nullptr;
    // }

    void changeEffect(std::shared_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    std::shared_ptr<Effect> getEffect() override;

    ~SimpleRenderer() override {
        delete[] effectArray;
        effectArray = nullptr;
        if (currentEffect) currentEffect.reset();
        pixelMapper = nullptr;
    }
};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
