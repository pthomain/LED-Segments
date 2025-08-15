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

#ifndef UMBRELLA_LAYOUT_CONFIG_H
#define UMBRELLA_LAYOUT_CONFIG_H

#include "lib/engine/displayspec/config/LayoutConfig.h"

enum UmbrellaLayout {
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static const std::set<uint16_t> umbrellaLayoutIds = {
    {LEDS_IN_SPOKE, SPOKES_IN_WHOLE}
};

static const std::map<uint16_t, String> umbrellaLayoutNames = {
    {LEDS_IN_SPOKE, "LEDS_IN_SPOKE"},
    {SPOKES_IN_WHOLE, "SPOKES_IN_WHOLE"},
};

static WeightedLayouts umbrellaLayoutSelector(RenderableType type) {
    switch (type) {
        case RenderableType::EFFECT:
            return {
                {LEDS_IN_SPOKE, 4},
                {SPOKES_IN_WHOLE, 1}
            };

        case RenderableType::OVERLAY:
        case RenderableType::TRANSITION:
        default:
            return {{LEDS_IN_SPOKE, 1}};
    }
}

#endif //UMBRELLA_LAYOUT_CONFIG_H
