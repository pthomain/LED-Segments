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
#include "engine/effect/BaseEffectFactory.h"
#include "engine/utils/Weights.h"

class FadeTransition : public Effect<FadeTransition, uint8_t> {
public:
    explicit FadeTransition(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        uint8_t *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "FadeTransition"; }
    static WeightedOperations operations() { return just(EffectOperation::TRANSITION); }
    static EffectFactoryRef<uint8_t> factory;
};

class FadeTransitionFactory : public EffectFactory<FadeTransition, uint8_t> {};

#endif //FADETRANSITION_H
