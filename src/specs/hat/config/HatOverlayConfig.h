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

#ifndef HAT_OVERLAY_CONFIG_H
#define HAT_OVERLAY_CONFIG_H

#include "engine/displayspec/LayoutConfig.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/none/NoOverlay.h"
#include "overlays/wave/WaveOverlay.h"
#include "specs/hat/overlays/CompositeOverlay.h"
#include "specs/hat/overlays/EyeOverlay.h"

static EffectAndMirrors<CRGB> hatOverlaySelector(uint16_t layoutId) {
    return {
        {
            // {MoireOverlay::factory, 4},
            // {ChaseOverlay::factory, 5},
            // {WaveOverlay::factory, 4},
            // {DashOverlay::factory, 3},
            // {MatrixOverlay::factory, 3},
            {SparkleOverlay::factory, 1},
            {NoOverlay::factory, 3},
        },
        [](EffectFactoryRef<CRGB> overlayFactory) {
            return noMirrors(overlayFactory);
        }
    };
}

#endif //HAT_OVERLAY_CONFIG_H
