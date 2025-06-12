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

#ifndef UMBRELLALAYOUTCONFIG_H
#define UMBRELLALAYOUTCONFIG_H

#include <map>
#include <vector>
#include "effects/noise/NoiseEffect.h"
#include "effects/gradient/GradientEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "effects/slide/SlideEffect.h"
#include "engine/displayspec/LayoutCatalog.h"
#include "engine/effect/Effect.h"
#include "engine/overlay/none/NoOverlay.h"
#include "engine/transitions/Transition.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"

enum UmbrellaLayout {
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static const std::vector<uint16_t> umbrellaLayouts = std::vector<uint16_t>{
    LEDS_IN_SPOKE,
    LEDS_IN_SPOKE,
    LEDS_IN_SPOKE,
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static std::map<uint16_t, std::vector<EffectFactory<CRGB> > > umbrellaEffects() {
    return mapLayoutIndex<EffectFactory<CRGB> >(
        umbrellaLayouts,
        [](uint16_t layoutIndex) {
            return std::vector{
                SwirlEffect::factory,
                NoiseEffect::factory,
                SlideEffect::factory,
            };
        }
    );
}

static std::map<uint16_t, std::vector<EffectFactory<CRGB> > > umbrellaOverlays() {
    return mapLayoutIndex<EffectFactory<CRGB> >(
        umbrellaLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_SPOKE:
                    return std::vector{
                        // MoireOverlay::factory,
                        ChaseOverlay::factory,
                        DashOverlay::factory,
                        SparkleOverlay::factory,
                    };
                default: return NO_OVERLAYS;
            }
        }
    );
}


static std::map<uint16_t, std::vector<Mirror> > umbrellaMirrors() {
    return mapLayoutIndex<Mirror>(
        umbrellaLayouts,
        [](uint16_t layoutIndex) {
            switch (layoutIndex) {
                case LEDS_IN_SPOKE: return ALL_UNREPEATED_MIRRORS;
                default: return NO_MIRRORS;
            }
        }
    );
}

static std::map<uint16_t, std::vector<EffectFactory<uint8_t> > > umbrellaTransitions() {
    return mapLayoutIndex<EffectFactory<uint8_t> >(
        umbrellaLayouts,
        [](uint16_t layoutIndex) {
            return std::vector{
                SlideTransition::factory,
                FadeTransition::factory
            };
        }
    );
}

static LayoutCatalog umbrellaLayoutCatalog() {
    return LayoutCatalog(
        umbrellaLayouts,
        {
            {LEDS_IN_SPOKE, "LEDS_IN_SPOKE"},
            {SPOKES_IN_WHOLE, "SPOKES_IN_WHOLE"},
        },
        umbrellaEffects(),
        umbrellaOverlays(),
        umbrellaTransitions(),
        umbrellaMirrors(),
        1.0f //0.25f
    );
}

#endif //UMBRELLALAYOUTCONFIG_H
