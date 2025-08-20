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

#ifndef TEST_OVERLAY_CONFIG_H
#define TEST_OVERLAY_CONFIG_H

#include "LayoutConfig.h"
#include "engine/displayspec/config/LayoutConfig.h"

//Library-provided overlays below, custom overlays can be created in the same way as effects, see CustomEffect.h for an example
#include "overlays/none/NoOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/none/NoOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"

// Defines which overlays are randomly picked for the given layout.
// Overlays are added on top of effects and applied using an mixing operation
// (usually multiply, which affects the alpha value of the effect's pixels)
static RenderablesAndMirrors<CRGB> overlaySelector(uint16_t layoutId) {
    switch (layoutId) {
        // case GROUP_BY_1:
        // case GROUP_BY_2:
        //     return RenderablesAndMirrors<CRGB>(
        //         {
        //             // Which overlays to use for the GROUP_BY_1 and GROUP_BY_2 layout (and their weights)
        //             {SparkleOverlay::factory, 5},
        //             {DashOverlay::factory, 4},
        //             {NoOverlay::factory, 2},
        //         },
        //         MirrorSelector<CRGB>([](RenderableFactoryRef<CRGB> overlayFactory) {
        //             //For DashOverlay, only apply Mirror::NONE and Mirror::REVERSE
        //             if (overlayFactory->is<DashOverlay>()) {
        //                 return WeightedMirrors{
        //                     {Mirror::NONE, 2},
        //                     {Mirror::REVERSE, 1},
        //                 };
        //             }
        //             return allMirrors<CRGB>(overlayFactory); //apply any mirror to the other overlays
        //         })
        //     );

        default:
            return {
                just(NoOverlay::factory),  //use NoOverlay for remaining layouts
                noMirrors<CRGB>         //No mirrors applied to NoOverlay
            };
    }
}

#endif // TEST_OVERLAY_CONFIG_H
