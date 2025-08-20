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

#ifndef LED_SEGMENTS_CHAOSEFFECT_H
#define LED_SEGMENTS_CHAOSEFFECT_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"

namespace LEDSegments {

class ChaosEffect : public Effect<ChaosEffect> {
    uint8_t paletteIncrement;
    std::unique_ptr<uint8_t[]> segmentRotationSpeeds;
    std::unique_ptr<uint8_t[]> segmentDirections;
    std::unique_ptr<uint8_t[]> segmentInitialPositions;

public:
    static const uint8_t PARAM_PALETTE_INCREMENT = 0; // Base palette increment

    explicit ChaosEffect(const RenderableContext &context);

    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentArraySize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) override;

    static constexpr const char *name() { return "ChaosEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class ChaosEffectFactory : public RenderableFactory<ChaosEffectFactory, ChaosEffect, CRGB> {
public:
    static Params declareParams() {
        return {
            {ChaosEffect::PARAM_PALETTE_INCREMENT, random8(1, 5)}   // Random palette increment
        };
    }
};

} // LEDSegments

#endif //LED_SEGMENTS_CHAOSEFFECT_H