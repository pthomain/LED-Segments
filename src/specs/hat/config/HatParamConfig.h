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

#ifndef HAT_PARAM_CONFIG_H
#define HAT_PARAM_CONFIG_H

#include "engine/displayspec/config/LayoutConfig.h"

static std::map<uint8_t, uint16_t> hatOverlayParamSelector(
    TypeInfo::ID renderableId,
    Mirror mirror
) {
    return {};
}

static std::map<uint8_t, uint16_t> hatParamSelector(
    RenderableType type,
    TypeInfo::ID renderableId,
    uint16_t layoutId,
    Mirror mirror
) {
    switch (type) {
        case RenderableType::OVERLAY:
            return hatOverlayParamSelector(renderableId, mirror);
        default:
            return {};
    }
}

#endif //HAT_PARAM_CONFIG_H
