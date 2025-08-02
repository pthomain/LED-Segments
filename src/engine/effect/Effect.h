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
protected:
    uint16_t param(uint8_t paramKey) const {
        auto parameters = this->context.parameters;
        if (parameters.find(paramKey) == parameters.end()) {
            Serial.printf("Effect %s: Parameter %d not found\n", Child::name(), paramKey);
            return 0;
        }

        return parameters.at(paramKey);
    }

public :
    explicit Effect(const EffectContext &effectContext) : BaseEffect<C>(effectContext) {
        static_assert(
            std::is_base_of_v<Effect, Child>,
            "Child must be a subclass of Effect"
        );

        static_assert(
            std::is_same_v<decltype(Child::name()), const char *>,
            "Child class must implement static const char* name()"
        );

        static_assert(
            std::is_same_v<decltype(std::declval<Child>().operations()), WeightedOperations>,
            "Child class must implement WeightedOperations operations()"
        );
    };

    void onEachFrame(
        float progress,
        unsigned long timeElapsedInMillis
    ) override {
    }

    WeightedOperations effectOperations() override {
        return static_cast<Child *>(this)->operations();
    }

    static const char *name() {
        return Child::name();
    }
};

template<typename E, typename C>
using EffectAndName = std::pair<std::unique_ptr<Effect<E, C> >, const char *>;

#endif //EFFECTS_H
