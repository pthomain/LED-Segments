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

#include "effects/noise/NoiseEffect.h"
#include "effects/gradient/GradientEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "effects/slide/SlideEffect.h"
#include "engine/displayspec/LayoutCatalog.h"
#include "engine/effect/Effect.h"
#include "engine/overlay/none/NoOverlay.h"
#include "engine/transitions/Transition.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"

enum UmbrellaLayout {
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static const std::vector<uint16_t> umbrellaLayouts = {
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static std::pair<WeightedEffects<CRGB>, MirrorSelector<CRGB> > umbrellaEffectSelector(uint16_t layoutIndex) {
    return {
        {
            {GradientEffect::factory, 1},
            {SwirlEffect::factory, 1},
            {NoiseEffect::factory, 1},
            {SlideEffect::factory, 1}
        },
        allCRGBMirrors
    };
}

static std::pair<WeightedEffects<CRGB>, MirrorSelector<CRGB> > umbrellaOverlaySelector(uint16_t layoutIndex) {
    switch (layoutIndex) {
        case LEDS_IN_SPOKE:
            return {
                {
                    {MoireOverlay::factory, 1},
                    {ChaseOverlay::factory, 1},
                    {DashOverlay::factory, 1},
                },
                allCRGBMirrors
            };
        default: return NO_OVERLAYS;
    }
}

static std::pair<WeightedEffects<uint8_t>, MirrorSelector<uint8_t> > umbrellaTransitionSelector(uint16_t layoutIndex) {
    return {
        {
            {SlideTransition::factory, 1},
            {FadeTransition::factory, 1},
        },
        allIntMirrors
    };
}

static LayoutCatalog umbrellaLayoutCatalog() {
    return LayoutCatalog(
        umbrellaLayouts,
        {
            {LEDS_IN_SPOKE, "LEDS_IN_SPOKE"},
            {SPOKES_IN_WHOLE, "SPOKES_IN_WHOLE"},
        },
        umbrellaEffectSelector,
        umbrellaOverlaySelector,
        umbrellaTransitionSelector
    );
}

#endif //UMBRELLALAYOUTCONFIG_H
