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

#include "engine/displayspec/config/LayoutConfig.h"
#include "overlays/none/NoOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/wave/WaveOverlay.h"
#include "overlays/matrix/MatrixOverlay.h"
#include "FibonacciLayoutDefinitions.h"

static RenderablesAndMirrors<CRGB> fibonacciOverlaySelector(uint16_t layoutId) {
    const auto [pixelUnit, direction, alignment, inflexion] = layoutInfo(layoutId);

    if (pixelUnit == SEGMENT) {
        if (alignment == SPIRAL || alignment == RADIAL) {
            return {
                {
                    // {MoireOverlay::factory, 4},
                    // {ChaseOverlay::factory, 5},
                    {WaveOverlay::factory, 4},
                    // {DashOverlay::factory, 3},
                    {MatrixOverlay::factory, 3},
                },
                [](RenderableFactoryRef<CRGB> overlayFactory) {
                    return WeightedMirrors{
                        {Mirror::OVERLAY_REPEAT_2, 1},
                        {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},

                        {Mirror::OVERLAY_REPEAT_3, 1},
                        {Mirror::OVERLAY_REPEAT_3_REVERSE, 1},

                        {Mirror::OVERLAY_REPEAT_4, 1},
                        {Mirror::OVERLAY_REPEAT_4_REVERSE, 1},

                        {Mirror::OVERLAY_REPEAT_6, 1},
                        {Mirror::OVERLAY_REPEAT_6_REVERSE, 1},
                    };
                }
            };
        }
    } else {
        //PIXEL
        if (alignment == SPIRAL || alignment == RADIAL) {
            return {
                {
                    {MoireOverlay::factory, 4},
                    {ChaseOverlay::factory, 5},
                    {WaveOverlay::factory, 4},
                    {DashOverlay::factory, 3},
                    {MatrixOverlay::factory, 3},
                },
                [](RenderableFactoryRef<CRGB> overlayFactory) {
                    if (overlayFactory->is<MatrixOverlay>()) {
                        return WeightedMirrors{
                            {Mirror::NONE, 2},
                            {Mirror::REVERSE, 1},
                        };
                    }

                    if (overlayFactory->is<ChaseOverlay>()) {
                        return unrepeatedMirrors<CRGB>(overlayFactory);
                    }

                    if (overlayFactory->is<MoireOverlay>()) {
                        return WeightedMirrors{
                            {Mirror::NONE, 2},
                            {Mirror::REVERSE, 2},
                            {Mirror::CENTRE, 2},
                            {Mirror::EDGE, 2},

                            {Mirror::REPEAT, 1},
                            {Mirror::REPEAT_REVERSE, 1},

                            {Mirror::OVERLAY_REVERSE, 1},
                            {Mirror::OVERLAY_REPEAT_2, 1},
                            {Mirror::OVERLAY_REPEAT_3, 1},
                            {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},
                            {Mirror::OVERLAY_REPEAT_3_REVERSE, 1},
                        };
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

                            {Mirror::REPEAT_REVERSE, 2},
                            {Mirror::OVERLAY_REVERSE, 1},

                            {Mirror::OVERLAY_REPEAT_2, 1},
                            {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},

                            {Mirror::OVERLAY_REPEAT_3, 1},
                            {Mirror::OVERLAY_REPEAT_3_REVERSE, 1},
                        };
                    }

                    return noMirrors(overlayFactory);
                }
            };
        }
    }
    return {};
}

#endif //FIBONACCI_OVERLAY_CONFIG_H