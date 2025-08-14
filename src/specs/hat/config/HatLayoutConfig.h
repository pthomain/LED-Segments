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

#ifndef HAT_LAYOUT_CONFIG_H
#define HAT_LAYOUT_CONFIG_H

#include "engine/displayspec/LayoutConfig.h"

constexpr uint8_t NB_PANELS = 5;
constexpr uint8_t LEDS_PER_PANEL = 6;
constexpr uint8_t LEDS_PER_EYE = 37;
constexpr uint8_t EYE_CIRCLES = 4;

enum HatLayout {
    CONCENTRIC_EYES,
    LINEAR_EYES,
};

static const std::set<uint16_t> hatLayoutIds = {
    {
        CONCENTRIC_EYES,
        LINEAR_EYES
    }
};

static const std::map<uint16_t, String> hatLayoutNames = {
    {CONCENTRIC_EYES, "CONCENTRIC_EYES"},
    {LINEAR_EYES, "LINEAR_EYES"}
};

static WeightedLayouts hatLayoutSelector(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
        case EffectType::TRANSITION:
            return {
                {LINEAR_EYES, 1},
                {CONCENTRIC_EYES, 1}
            };

        case EffectType::OVERLAY:
        default:
            return just(uint16_t(LINEAR_EYES));
    }
}

#endif //HAT_LAYOUT_CONFIG_H
