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

enum HatLayout {
    EYE_CONCENTRIC,
    EYE_SPIRAL,
    EYE_RADIAL,
    EYE_ROW,
    EYE_COLUMN,
    EYE_DIAGONAL_TOP_RIGHT,
    EYE_DIAGONAL_TOP_LEFT,
    EYE_DIAGONAL_BOTTOM_RIGHT,
    EYE_DIAGONAL_BOTTOM_LEFT,
    EYE_LINEAR,
};

static const std::set<uint16_t> hatLayoutIds = {
    {
        EYE_CONCENTRIC,
        EYE_SPIRAL,
        EYE_RADIAL,
        EYE_ROW,
        EYE_COLUMN,
        EYE_DIAGONAL_TOP_RIGHT,
        EYE_DIAGONAL_TOP_LEFT,
        EYE_DIAGONAL_BOTTOM_RIGHT,
        EYE_DIAGONAL_BOTTOM_LEFT,
        EYE_LINEAR
    }
};

static const std::map<uint16_t, String> hatLayoutNames = {
    {EYE_CONCENTRIC, "EYE_CONCENTRIC"},
    {EYE_SPIRAL, "EYE_SPIRAL"},
    {EYE_RADIAL, "EYE_RADIAL"},
    {EYE_ROW, "EYE_ROW"},
    {EYE_COLUMN, "EYE_COLUMN"},
    {EYE_DIAGONAL_TOP_RIGHT, "EYE_DIAGONAL_TOP_RIGHT"},
    {EYE_DIAGONAL_TOP_LEFT, "EYE_DIAGONAL_TOP_LEFT"},
    {EYE_DIAGONAL_BOTTOM_RIGHT, "EYE_DIAGONAL_BOTTOM_RIGHT"},
    {EYE_DIAGONAL_BOTTOM_LEFT, "EYE_DIAGONAL_BOTTOM_LEFT"},
    {EYE_LINEAR, "EYE_LINEAR"}
};

static WeightedLayouts hatLayoutSelector(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
        case EffectType::TRANSITION:
            return {
                {EYE_ROW, 1},
                {EYE_COLUMN, 1},
                {EYE_DIAGONAL_TOP_RIGHT, 1},
                {EYE_DIAGONAL_TOP_LEFT, 1},
                {EYE_DIAGONAL_BOTTOM_RIGHT, 1},
                {EYE_DIAGONAL_BOTTOM_LEFT, 1},
                {EYE_SPIRAL, 12},
                {EYE_RADIAL, 24},
                {EYE_CONCENTRIC, 6}
            };

        case EffectType::OVERLAY:
        default:
            return just(uint16_t(EYE_LINEAR));
    }
}

#endif //HAT_LAYOUT_CONFIG_H
