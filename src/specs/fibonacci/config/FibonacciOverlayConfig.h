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

#ifndef FIBONACCI_OVERLAY_CONFIG_H
#define FIBONACCI_OVERLAY_CONFIG_H

#include <engine/displayspec/LayoutConfig.h>
#include "overlays/none/NoOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/wave/WaveOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "FibonacciLayoutDefinitions.h"

static EffectAndMirrors<CRGB> fibonacciOverlaySelector(uint16_t layoutId) {
    // if (getAlignment(layoutId) == RADIAL) {
    return {
        {
            // {MoireOverlay::factory, 4},
            // {ChaseOverlay::factory, 4},
            // {WaveOverlay::factory, 3},
            // {DashOverlay::factory, 2},
        },
        [](EffectFactoryRef<CRGB> overlayFactory) {
            if (
                overlayFactory->is<MoireOverlay>()
                || overlayFactory->is<ChaseOverlay>()
            ) {
                return noMirrors<CRGB>(overlayFactory);
                // return WeightedMirrors{
                //     {Mirror::NONE, 2},
                //     {Mirror::REVERSE, 2},
                //     {Mirror::CENTRE, 2},
                //     {Mirror::EDGE, 2},
                //
                //     {Mirror::REPEAT, 1},
                //     {Mirror::REPEAT_REVERSE, 1},
                //
                //     {Mirror::OVERLAY_REVERSE, 1},
                //     {Mirror::OVERLAY_REPEAT_2, 1},
                //     {Mirror::OVERLAY_REPEAT_3, 1},
                //     {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},
                //     {Mirror::OVERLAY_REPEAT_3_REVERSE, 1},
                // };
            }

            if (overlayFactory->is<DashOverlay>()) {
                return WeightedMirrors{
                    {Mirror::NONE, 2},
                    {Mirror::REVERSE, 2},
                    {Mirror::CENTRE, 2},
                    {Mirror::EDGE, 2},

                    {Mirror::REPEAT, 1},
                    {Mirror::REPEAT_REVERSE, 1},

                    {Mirror::OVERLAY_REVERSE, 1},
                    {Mirror::OVERLAY_REPEAT_2, 1},
                };
            }

            if (overlayFactory->is<WaveOverlay>()) {
                return WeightedMirrors{
                    {Mirror::NONE, 3},
                    {Mirror::REVERSE, 3},
                    {Mirror::CENTRE, 3},
                    {Mirror::EDGE, 3},
                    {Mirror::REPEAT, 2},

                    {Mirror::REPEAT_REVERSE, 2},
                    {Mirror::OVERLAY_REVERSE, 1},

                    {Mirror::OVERLAY_REPEAT_2, 1},
                    {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},
                };
            }
        }
    };
    // }

    return {just(NoOverlay::factory), allMirrors<CRGB>};
}

#endif //FIBONACCI_OVERLAY_CONFIG_H
