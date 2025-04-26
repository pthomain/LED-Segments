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

#include "engine/displayspec/LayoutCatalog.h"
#include <engine/effect/none/NoEffect.h>
#include <engine/mirror/Mirror.h>

#include "specs/fibonacci/FibonacciLayoutConfig.h"

const EffectFactory &LayoutCatalog::randomEffectFactory(uint16_t layoutIndex) const {
    return randomMapEntryForLayout(EFFECT_ENTRY, layoutIndex, _effects, NoEffect::factory);
}

const Mirror LayoutCatalog::randomMirror(uint16_t layoutIndex) const {
    return randomMapEntryForLayout(MIRROR_ENTRY, layoutIndex, _mirrors, Mirror::NONE);
}

const std::pair<uint16_t, Transition> LayoutCatalog::randomTransition() const {
    if (_transitions.empty()) {
        if constexpr (IS_DEBUG) Serial.println("Transition map is empty");
        return {0, Transition::NONE};
    }

    auto iterator = _transitions.begin();
    std::advance(iterator, random8(_transitions.size()));

    uint16_t transitionLayoutIndex = iterator->first;
    std::vector<Transition> transitions = iterator->second;

    if (transitions.empty()) {
        if constexpr (IS_DEBUG) {
            Serial.println("No transition values provided for layout ");
            Serial.println(transitionLayoutIndex);
        }
        return {0, Transition::NONE};
    }

    Transition transition = transitions.at(random8(transitions.size()));

    return {
        transitionLayoutIndex,
        transition
    };
}
