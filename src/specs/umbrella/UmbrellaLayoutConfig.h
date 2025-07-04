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
#include "overlays/sparkle/SparkleOverlay.h"

enum UmbrellaLayout {
    LEDS_IN_SPOKE,
    SPOKES_IN_WHOLE
};

static const std::set<uint16_t> umbrellaLayoutIds = {
    {LEDS_IN_SPOKE, SPOKES_IN_WHOLE}
};

static const WeightedLayouts umbrellaLayoutSelector(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
            return {
                {LEDS_IN_SPOKE, 4},
                {SPOKES_IN_WHOLE, 1}
            };

        case EffectType::OVERLAY:
        case EffectType::TRANSITION:
        default:
            return {{LEDS_IN_SPOKE, 1}};
    }
};

static EffectAndMirrors<CRGB> umbrellaEffectSelector(uint16_t layoutId) {
    if (layoutId == LEDS_IN_SPOKE) {
        return {
            {
                {SwirlEffect::factory, 3},
                {NoiseEffect::factory, 2},
                {SlideEffect::factory, 1}
            },
            allCRGBMirrors
        };
    } else {
        return {
            {
                {SwirlEffect::factory, 1},
            },
            [](EffectFactoryRef<CRGB> effectFactory) {
                return WeightedMirrors{
                    {Mirror::NONE, 1},
                    {Mirror::REVERSE, 1}
                };
            }
        };
    }
}

static EffectAndMirrors<CRGB> umbrellaOverlaySelector(uint16_t layoutId) {
    if (layoutId == LEDS_IN_SPOKE) {
        return {
            {
                {MoireOverlay::factory, 3},
                {ChaseOverlay::factory, 3},
                {DashOverlay::factory, 2},
                {SparkleOverlay::factory, 1},
                {NoOverlay::factory, 1},
            },
            [](EffectFactoryRef<CRGB> overlayFactory) {
                if (overlayFactory->name() == MoireOverlay::name()) {
                    return WeightedMirrors{}; //No mirrors for this overlay
                }
                return allCRGBMirrors(overlayFactory);
            }
        };
    }

    return {}; //No overlays for SPOKES_IN_WHOLE
}

static EffectAndMirrors<uint8_t> umbrellaTransitionSelector(uint16_t layoutId) {
    return {
        {
            //TODO the first 4 pixels of the first segment flash white when using transitions
            // {SlideTransition::factory, 1},
            {NoTransition::factory, 1},
        },
        allIntMirrors
    };
}

static LayoutCatalog umbrellaLayoutCatalog() {
    return LayoutCatalog(
        umbrellaLayoutIds,
        {
            {LEDS_IN_SPOKE, "LEDS_IN_SPOKE"},
            {SPOKES_IN_WHOLE, "SPOKES_IN_WHOLE"},
        },
        umbrellaLayoutSelector,
        umbrellaEffectSelector,
        umbrellaOverlaySelector,
        umbrellaTransitionSelector
    );
}

#endif //UMBRELLALAYOUTCONFIG_H
