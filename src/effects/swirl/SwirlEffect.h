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

#ifndef LED_SEGMENTS_SWIRLEFFECT_H
#define LED_SEGMENTS_SWIRLEFFECT_H

#include "engine/render/renderable/BaseRenderableFactory.h"
#include "engine/render/renderable/TypedRenderable.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Weights.h"

namespace LEDSegments {

class SwirlEffect : public Effect<SwirlEffect> {
    const bool isReversible;
    const bool isReversed = isReversible ? probability(0.5f) : false;
    const uint8_t speedDivider;

public:
    static const uint8_t PARAM_IS_REVERSIBLE = 0;
    static const uint8_t PARAM_SPEED_DIVIDER = 1;

    explicit SwirlEffect(const RenderableContext &context): Effect(context),
          isReversible(param(PARAM_IS_REVERSIBLE) > 0),
          speedDivider(max(1, param(PARAM_SPEED_DIVIDER))) {
    }

    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "SwirlEffect"; }
    static RenderableFactoryRef<CRGB> factory;
};

class SwirlEffectFactory : public RenderableFactory<SwirlEffectFactory, SwirlEffect, CRGB> {
public:
    static Params declareParams() {
        return {
            {SwirlEffect::PARAM_IS_REVERSIBLE, 1}, // 0 for false
            {SwirlEffect::PARAM_SPEED_DIVIDER, 4} // 0 for false
        };
    }
};

} // namespace LEDSegments

#endif //LED_SEGMENTS_SWIRLEFFECT_H
