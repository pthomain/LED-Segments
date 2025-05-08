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

#ifndef FADETRANSITION_H
#define FADETRANSITION_H
#include "engine/effect/Effect.h"

class FadeTransition : public Effect<uint8_t>, public Effect<uint8_t>::Factory<FadeTransition> {
public:
    explicit FadeTransition(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        uint8_t *effectArray,
        uint16_t effectArraySize,
        float progress,
        unsigned long time
    ) override;

    String name() const override { return "Fade"; }
    EffectType type() const override { return EffectType::TRANSITION; }

    static EffectFactory<uint8_t> factory;
};

#endif //FADETRANSITION_H
