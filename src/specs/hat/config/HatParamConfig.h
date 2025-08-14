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

#include "engine/displayspec/LayoutConfig.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"

static std::map<uint8_t, uint16_t> hatOverlayParamSelector(
    TypeInfo::ID effectId,
    Mirror mirror
) {
    if (ChaseOverlay::factory->is(effectId)) {
        return ChaseOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case ChaseOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 0;
                case ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    if (DashOverlay::factory->is(effectId)) {
        return DashOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case DashOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 0;
                case DashOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    if (MoireOverlay::factory->is(effectId)) {
        return MoireOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case MoireOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 0;
                case MoireOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    if (SparkleOverlay::factory->is(effectId)) {
        return SparkleOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case SparkleOverlay::PARAM_DENSITY: return 50;
                default: return defaultValue;
            }
        });
    }

    if (WaveOverlay::factory->is(effectId)) {
        return WaveOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case WaveOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 0;
                case WaveOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }
    if (MatrixOverlay::factory->is(effectId)) {
        return MatrixOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case MatrixOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 0;
                case MatrixOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    return {};
}

static std::map<uint8_t, uint16_t> hatParamSelector(
    EffectType effectType,
    TypeInfo::ID effectId,
    Mirror mirror
) {
    switch (effectType) {
        case EffectType::OVERLAY:
            return hatOverlayParamSelector(effectId, mirror);
        default:
            return {};
    }
}

#endif //HAT_PARAM_CONFIG_H
