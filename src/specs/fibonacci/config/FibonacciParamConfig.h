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

#ifndef FIBONACCI_PARAM_CONFIG_H
#define FIBONACCI_PARAM_CONFIG_H

#include "engine/utils/Utils.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/gradient/GradientEffect.h>
#include <effects/swirl/SwirlEffect.h>
#include <effects/slide/SlideEffect.h>
#include "overlays/none/NoOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/wave/WaveOverlay.h"
#include "FibonacciLayoutDefinitions.h"
#include "overlays/sparkle/SparkleOverlay.h"

static std::map<uint8_t, uint16_t> fibonacciEffectParamSelector(
    TypeInfo::ID effectId,
    Mirror mirror
) {
    if (GradientEffect::factory->is(effectId)) {
        return GradientEffect::factory->params([](uint8_t paramKey, uint16_t defaultValue)-> uint16_t {
            switch (paramKey) {
                case GradientEffect::PARAM_COLOUR_START: return random8();
                case GradientEffect::PARAM_DENSITY_VARIATION: return random8(85);
                default: return defaultValue;
            }
        });
    }

    if (NoiseEffect::factory->is(effectId)) {
        return NoiseEffect::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case NoiseEffect::PARAM_PALETTE_SCALE: return random8(2, 6);
                case NoiseEffect::PARAM_NOISE_SCALE: return random8(10, 20);
                case NoiseEffect::PARAM_NOISE_SPEED: return random8(5, 100);
                case NoiseEffect::PARAM_SPEED_DIVIDER: return 5;
                default: return defaultValue;
            }
        });
    }

    if (SlideEffect::factory->is(effectId)) {
        return SlideEffect::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case SlideEffect::PARAM_NB_COLOURS: return 4;
                case SlideEffect::PARAM_COLOUR_START: return random8();
                case SlideEffect::PARAM_SPEED_DIVIDER: return 50;
                default: return defaultValue;
            }
        });
    }

    if (SwirlEffect::factory->is(effectId)) {
        return SwirlEffect::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case SwirlEffect::PARAM_IS_REVERSIBLE: return 1;
                case SwirlEffect::PARAM_SPEED_DIVIDER: return 4;
                default: return defaultValue;
            }
        });
    }

    return {};
}

static std::map<uint8_t, uint16_t> fibonacciOverlayParamSelector(
    TypeInfo::ID effectId,
    Mirror mirror
) {
    if (ChaseOverlay::factory->is(effectId)) {
        return ChaseOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT: return 1;
                case ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT: return 5;
                case ChaseOverlay::PARAM_DISTANCE_BETWEEN_SPARKS: return 10;
                case ChaseOverlay::PARAM_TRAIL_LENGTH: return 3;
                case ChaseOverlay::PARAM_CHANCE_OF_BOUNCE: return 75;
                case ChaseOverlay::PARAM_CHANCE_OF_SWIRL: return 50;
                case ChaseOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 4;
                case ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    if (DashOverlay::factory->is(effectId)) {
        return DashOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case DashOverlay::PARAM_TAIL_SPEED: return random8(1, 5);
                case DashOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 4;
                case DashOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    if (MoireOverlay::factory->is(effectId)) {
        return MoireOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case MoireOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 4;
                case MoireOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                case MoireOverlay::PARAM_HEAD_LENGTH: return 5;
                case MoireOverlay::PARAM_IS_CLOCKWISE: return probability(0.5f);
                case MoireOverlay::PARAM_IS_MASK_INCLUSIVE: return probability(0.5f);
                default: return defaultValue;
            }
        });
    }

    if (SparkleOverlay::factory->is(effectId)) {
        return SparkleOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case SparkleOverlay::PARAM_DENSITY: return 5;
                default: return defaultValue;
            }
        });
    }

    if (WaveOverlay::factory->is(effectId)) {
        return WaveOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case WaveOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT: return 4;
                case WaveOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 1;
                default: return defaultValue;
            }
        });
    }

    return {};
}

static std::map<uint8_t, uint16_t> fibonacciParamSelector(
    EffectType effectType,
    TypeInfo::ID effectId,
    Mirror mirror
) {
    switch (effectType) {
        case EffectType::EFFECT:
            return fibonacciEffectParamSelector(effectId, mirror);
        case EffectType::OVERLAY:
            return fibonacciOverlayParamSelector(effectId, mirror);
        default:
            return {};
    }
}

#endif //FIBONACCI_PARAM_CONFIG_H
