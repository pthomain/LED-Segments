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

#ifndef NOOVERLAY_H
#define NOOVERLAY_H

#include "engine/effect/Effect.h"

class NoOverlay : public Effect, public Effect::Factory<NoOverlay> {
public:
    explicit NoOverlay(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        float progress,
        unsigned long time
    ) override;

    String name() const override { return "None"; }
    EffectType type() const override { return EffectType::OVERLAY_ALPHA; }

    static EffectFactory factory;
};

static const std::vector NO_OVERLAYS = {NoOverlay::factory};

#endif //NOOVERLAY_H
