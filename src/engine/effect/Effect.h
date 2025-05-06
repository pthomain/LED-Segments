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

#pragma once
#ifndef EFFECTS_H
#define EFFECTS_H

#include <memory>

#include "EffectContext.h"
#include "functional"
#include "engine/effect/EffectType.h"

constexpr uint8_t MIN_CYCLE_SPEED = 5;
constexpr uint8_t MAX_CYCLE_SPEED = 15;
constexpr uint8_t PALETTE_SIZE = 16;

class Effect {
    const unsigned long start;

protected:
    CRGBPalette16 palette;
    uint8_t randomStart;
    bool isArrayInitialised = false;

public :
    const EffectContext effectContext;

    explicit Effect(EffectContext effectContext) : effectContext(std::move(effectContext)),
                                                   start(millis()),
                                                   randomStart(random8()) {
        palette = effectContext.palette.palette();
    };

    void fillArray(
        CRGB *effectArray,
        uint16_t effectArraySize,
        float progress
    );

    virtual void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        float progress,
        unsigned long time
    ) = 0;

    virtual String name() const = 0;

    virtual EffectType type() const = 0;

    virtual ~Effect() = default;

    template<typename T>
    class Factory {
    public:
        static std::shared_ptr<Effect> createEffect(const EffectContext &effectContext) {
            return std::make_shared<Effect>(new T(effectContext));
        }
    };
};

using EffectFactory = std::function<std::unique_ptr<Effect>(const EffectContext &effectContext)>;
static const std::vector<EffectFactory> NO_EFFECTS = std::vector<EffectFactory>{};

#endif //EFFECTS_H
