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
#include "BaseEffect.h"
#include "EffectContext.h"
#include "engine/utils/Weights.h"

constexpr uint8_t MIN_CYCLE_SPEED = 5;
constexpr uint8_t MAX_CYCLE_SPEED = 15;
constexpr uint8_t PALETTE_SIZE = 16;

template<typename Child, typename C>
class Effect : public BaseEffect<C> {
public :
    explicit Effect(const EffectContext &effectContext) : BaseEffect<C>(effectContext) {
        static_assert(
            std::is_same<decltype(Child::name()), const char *>::value,
            "Child class must implement static constexpr const char* name()"
        );

        static_assert(
            std::is_same<decltype(Child::operations()), WeightedOperations>::value,
            "Child class must implement static constexpr const WeightedOperations operations()"
        );
    };

    const char *effectName() override {
        return name();
    }

    WeightedOperations effectOperations() override {
        return operations();
    }

    static const char *name() {
        return Child::name();
    }

    static WeightedOperations operations() {
        return Child::operations();
    }
};

template<typename E, typename C>
using EffectAndName = std::pair<std::unique_ptr<Effect<E, C> >, const char *>;

#endif //EFFECTS_H
