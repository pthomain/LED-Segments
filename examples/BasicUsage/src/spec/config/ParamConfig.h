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

#ifndef TEST_PARAM_CONFIG_H
#define TEST_PARAM_CONFIG_H

#include "effects/gradient/GradientEffect.h"
#include "engine/displayspec/config/LayoutConfig.h"
#include "overlays/chase/ChaseOverlay.h"

// Used to override the default parameters for individual effects (optional)
static std::map<uint8_t, uint16_t> effectParamSelector(
    TypeInfo::ID renderableId, //The effect's unique ID (Renderables can be effects, overlays or transitions)
    Mirror mirror //The mirror being applied to the effect, might influence the choice of parameters
) {
    // Overrides the default parameters for GradientEffect
    if (GradientEffect::factory->is(renderableId)) {
        return GradientEffect::factory->params([](uint8_t paramKey, uint16_t defaultValue)-> uint16_t {
            switch (paramKey) {
                case GradientEffect::PARAM_DENSITY_VARIATION: return random8(15);
                default: return defaultValue;
            }
        });
    }

    // Return empty map for other effects, using default parameters
    return {};
}

// Used to override the default parameters for individual effects (optional)
static std::map<uint8_t, uint16_t> overlayParamSelector(
    TypeInfo::ID renderableId,
    Mirror mirror
) {
    // Overrides the default parameters for ChaseOverlay
    if (ChaseOverlay::factory->is(renderableId)) {
        return ChaseOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT: return 1;
                case ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT: return 2;
                case ChaseOverlay::PARAM_DISTANCE_BETWEEN_SPARKS: return 1;
                case ChaseOverlay::PARAM_TRAIL_LENGTH: return 1;
                case ChaseOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 0;
                default: return defaultValue;
            }
        });
    }

    // Overrides the default parameters for DashOverlay
    if (DashOverlay::factory->is(renderableId)) {
        return ChaseOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case DashOverlay::PARAM_TAIL_SPEED: return 1;
                case DashOverlay::PARAM_OPERATION_INVERT_WEIGHT: return 0;
                default: return defaultValue;
            }
        });
    }

    //Return empty map for other overlays, using default parameters
    return {};
}

static std::map<uint8_t, uint16_t> paramSelector(
    RenderableType type,        //the type of renderable (effect, overlay or transition)
    TypeInfo::ID renderableId,  //its unique ID
    uint16_t layoutId,          //the layout used to render the renderable
    Mirror mirror               //the mirror used to render the renderable
) {
    switch (type) {
        case RenderableType::EFFECT:
            return effectParamSelector(renderableId, mirror);

        case RenderableType::OVERLAY:
            return overlayParamSelector(renderableId, mirror);

        default:
            return {}; //No parameters for the lib's transitions (custom user-defined transitions can use params)
    }
}

#endif // TEST_PARAM_CONFIG_H

