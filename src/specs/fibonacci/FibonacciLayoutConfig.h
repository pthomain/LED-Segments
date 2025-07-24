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

#ifndef LED_SEGMENTS_FIBONACCIENUMS_H
#define LED_SEGMENTS_FIBONACCIENUMS_H

#include <vector>
#include "Arduino.h"
#include "WString.h"
#include "engine/utils/Utils.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/gradient/GradientEffect.h>
#include <effects/swirl/SwirlEffect.h>
#include <effects/slide/SlideEffect.h>
#include <engine/displayspec/LayoutConfig.h>
#include "transitions/Transition.h"
#include "overlays/none/NoOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/wave/WaveOverlay.h"
#include "FibonacciLayoutDefinitions.h"
#include "overlays/sparkle/SparkleOverlay.h"

static WeightedLayouts fibonacciLayoutSelector(EffectType effectType) {
    auto ids = fibonacciLayoutIds();
    WeightedLayouts result;
    result.reserve(ids.size());

    std::transform(
        ids.begin(),
        ids.end(),
        std::back_inserter(result),
        [&](uint16_t layoutId) {
            switch (effectType) {
                case EffectType::EFFECT: {
                    if (getInflexion(layoutId) == INFLEXION_NONE) {
                        return WeightedLayout{layoutId, 1};
                    }
                    return WeightedLayout{layoutId, 0};
                }

                case EffectType::OVERLAY:
                    return WeightedLayout{layoutId, 0};

                case EffectType::TRANSITION:
                default:
                    return WeightedLayout{layoutId, 1};
            }
        }
    );

    return result;
}

static EffectAndMirrors<CRGB> fibonacciEffectSelector(uint16_t layoutId) {
    if (getAlignment(layoutId) == RADIAL) {
        return {
            {
                {GradientEffect::factory, 1},
                // {SwirlEffect::factory, 1},
                // {NoiseEffect::factory, 1},
                // {SlideEffect::factory, 1}
            },
            allMirrors<CRGB>
        };
    } else {
        return {
            {
                {GradientEffect::factory, 1},
                // {NoiseEffect::factory, 1},
            },
            noMirrors<CRGB>
        };
    }
}

static EffectAndMirrors<CRGB> fibonacciOverlaySelector(uint16_t layoutId) {
    if (getAlignment(layoutId) == RADIAL) {
        return {
            {
                {MoireOverlay::factory, 4},
                {ChaseOverlay::factory, 4},
                {WaveOverlay::factory, 3},
                {DashOverlay::factory, 2},
            },
            [](EffectFactoryRef<CRGB> overlayFactory) {
                if (
                    overlayFactory->is<MoireOverlay>()
                    || overlayFactory->is<ChaseOverlay>()
                ) {
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
                        {Mirror::REPEAT, 2},

                        {Mirror::REPEAT_REVERSE, 2},
                        {Mirror::OVERLAY_REVERSE, 1},

                        {Mirror::OVERLAY_REPEAT_2, 1},
                        {Mirror::OVERLAY_REPEAT_2_REVERSE, 1},
                    };
                }
            }
        };
    }

    return {just(NoOverlay::factory), allMirrors<CRGB>};
}

static EffectAndMirrors<uint8_t> fibonacciTransitionSelector(uint16_t layoutId) {
    return ALL_TRANSITIONS;
}

static std::map<uint8_t, uint16_t> fibonacciParamSelector(
    std::pair<TypeInfo::ID, Mirror> effectTypeAndMirror
) {
    const auto &[effectId, mirror] = effectTypeAndMirror;

    if (GradientEffect::factory->is(effectId)) {
        return GradientEffect::factory->params([](uint8_t paramKey) {
            switch (paramKey) {
                case GradientEffect::PARAM_START: return uint16_t(random8()); // Start hue
                case GradientEffect::PARAM_VARIATION: return uint16_t(random8(85)); // 33% variation
                default: return uint16_t(0);
            }
        });
    }

    // if (NoiseEffect::factory->is(effectId)) { return {{0, 0}}; }
    // if (SlideEffect::factory->is(effectId)) { return {{0, 0}}; }
    // if (SwirlEffect::factory->is(effectId)) { return {{0, 0}}; }
    // if (ChaseOverlay::factory->is(effectId)) { return {{0, 0}}; }
    // if (DashOverlay::factory->is(effectId)) { return {{0, 0}}; }
    // if (MoireOverlay::factory->is(effectId)) { return {{0, 0}}; }
    // if (SparkleOverlay::factory->is(effectId)) { return {{0, 0}}; }
    // if (WaveOverlay::factory->is(effectId)) { return {{0, 0}}; }
    // if (FadeTransition::factory->is(effectId)) { return {{0, 0}}; }
    // if (SlideTransition::factory->is(effectId)) { return {{0, 0}}; }

    return {};
}

static LayoutConfig fibonacciLayoutConfig() {
    auto layoutIds = fibonacciLayoutIds();
    auto names = std::map<uint16_t, String>();
    for (auto layoutId: layoutIds) {
        names.insert(std::pair(layoutId, getLayoutName(layoutId)));
    }

    return LayoutConfig(
        layoutIds,
        names,
        fibonacciLayoutSelector,
        fibonacciEffectSelector,
        fibonacciOverlaySelector,
        fibonacciTransitionSelector,
        fibonacciParamSelector
    );
}

#endif //LED_SEGMENTS_FIBONACCIENUMS_H
