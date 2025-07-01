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

#ifndef SPARKLEOVERLAY_H
#define SPARKLEOVERLAY_H
#include "engine/effect/Effect.h"

class SparkleOverlay : public Effect<SparkleOverlay, CRGB>{
    const float density = 0.005f;
    const CRGB minBrightness = CRGB(127, 127, 127);

public:
    explicit SparkleOverlay(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "SparkleOverlay"; }
    static constexpr EffectType type() { return EffectType::OVERLAY_MULTIPLY; }
    static const EffectFactory<CRGB>& factory;
};

class SparkleOverlayFactory : public EffectFactory<CRGB> {
public:
    std::unique_ptr<BaseEffect<CRGB> > create(const EffectContext &context) const override {
        return std::make_unique<SparkleOverlay>(context);
    }

    const char *name() const override {
        return SparkleOverlay::name();
    }
};

#endif //SPARKLEOVERLAY_H
