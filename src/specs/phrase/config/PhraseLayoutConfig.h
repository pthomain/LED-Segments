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

#ifndef PHRASELAYOUTCONFIG_H
#define PHRASELAYOUTCONFIG_H

#include "engine/displayspec/LayoutConfig.h"
#include "engine/effect/Effect.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/slide/SlideEffect.h>
#include "effects/gradient/GradientEffect.h"
#include "effects/swirl/SwirlEffect.h"
#include "overlays/none/NoOverlay.h"
#include "transitions/Transition.h"
#include "engine/utils/Weights.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "overlays/wave/WaveOverlay.h"

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,
    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,
    WORDS_IN_WHOLE
};

static const std::set<uint16_t> phraseLayouts = {
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,
    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,
    WORDS_IN_WHOLE
};

static WeightedLayouts phraseLayoutSelector(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 0},
                {LEDS_IN_LETTERS, 8},
                {LETTERS_IN_WHOLE, 12},
                {LETTERS_IN_WORDS, 0},
                {WORDS_IN_WHOLE, 8}
            };

        case EffectType::TRANSITION:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 1},
                {LEDS_IN_LETTERS, 4},
                {LETTERS_IN_WHOLE, 8},
                {LETTERS_IN_WORDS, 2},
                {WORDS_IN_WHOLE, 0}
            };

        case EffectType::OVERLAY:
            return {
                {LEDS_IN_WHOLE, 0},
                {LEDS_IN_WORDS, 2},
                {LEDS_IN_LETTERS, 4},
                {LETTERS_IN_WHOLE, 2},
                {LETTERS_IN_WORDS, 2},
                {WORDS_IN_WHOLE, 0}
            };
    }
}

static EffectAndMirrors<CRGB> phraseEffectSelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_WHOLE:
        case LEDS_IN_WORDS:
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WHOLE:
            return {
                {
                    {GradientEffect::factory, 2},
                    {SwirlEffect::factory, 8},
                    {NoiseEffect::factory, 4},
                    {SlideEffect::factory, 1}
                },
                [](EffectFactoryRef<CRGB> effectFactory) {
                    if (effectFactory->is<SlideEffect>()) {
                        return noMirrors(effectFactory);
                    }
                    return WeightedMirrors{
                        {Mirror::NONE, 1},
                        {Mirror::REVERSE, 1},
                        {Mirror::CENTRE, 2},
                        {Mirror::EDGE, 2}
                    };
                }
            };

        case LETTERS_IN_WORDS:
        case WORDS_IN_WHOLE:
            return {
                {
                    {GradientEffect::factory, 1},
                    {SwirlEffect::factory, 2},
                    {NoiseEffect::factory, 3}
                },
                noMirrors<CRGB>
            };
    }
};

static EffectAndMirrors<CRGB> phraseOverlaySelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
            return EffectAndMirrors<CRGB>{
                {
                    {WaveOverlay::factory, 1},
                    {NoOverlay::factory, 2},
                },
                undividedMirrors<CRGB>
            };

        case LEDS_IN_WORDS:
            return EffectAndMirrors<CRGB>{
                {
                    {WaveOverlay::factory, 2},
                    {DashOverlay::factory, 1},
                    {NoOverlay::factory, 4},
                },
                [](EffectFactoryRef<CRGB> overlayFactory) {
                    if (overlayFactory->is<ChaseOverlay>()) {
                        return allMirrors(overlayFactory);
                    }
                    if (overlayFactory->is<DashOverlay>()) {
                        return undividedMirrors(overlayFactory);
                    }
                    return unrepeatedMirrors(overlayFactory);
                }
            };

        case LETTERS_IN_WHOLE:
            return EffectAndMirrors<CRGB>{
                {
                    {ChaseOverlay::factory, 1},
                    {DashOverlay::factory, 1},
                    {NoOverlay::factory, 6},
                },
                [](EffectFactoryRef<CRGB> overlayFactory) {
                    if (overlayFactory->is<ChaseOverlay>()) {
                        return noMirrors(overlayFactory);
                    }
                    if (overlayFactory->is<DashOverlay>()) {
                        return WeightedMirrors{
                            {Mirror::NONE, 2},
                            {Mirror::CENTRE, 1}
                        };
                    }
                    return noMirrors(overlayFactory);
                }
            };

        case LEDS_IN_WHOLE:
        case LETTERS_IN_WORDS:
        case WORDS_IN_WHOLE:
        default: return {{}, noMirrors<CRGB>};
    }
};

static EffectAndMirrors<uint8_t> phraseTransitionSelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
        case LETTERS_IN_WHOLE:
            return {
                just(SlideTransition::factory),
                unrepeatedMirrors<uint8_t>
            };

        case LETTERS_IN_WORDS:
            return {
                just(SlideTransition::factory),
                undividedMirrors<uint8_t>
            };

        case LEDS_IN_WHOLE:
        case LEDS_IN_WORDS:
        case WORDS_IN_WHOLE:
        default:
            return {
                just(FadeTransition::factory),
                noMirrors<uint8_t>
            };
    }
}

static std::map<uint8_t, uint16_t> phraseParamSelector(
    TypeInfo::ID effectId,
    Mirror mirror
) {
    if (ChaseOverlay::factory->is(effectId)) {
        return ChaseOverlay::factory->params([](uint8_t paramKey, uint16_t defaultValue) -> uint16_t {
            switch (paramKey) {
                case ChaseOverlay::PARAM_MIN_SPARKS_PER_SEGMENT: return 1;
                case ChaseOverlay::PARAM_MAX_SPARKS_PER_SEGMENT: return 1;
                // case ChaseOverlay::PARAM_SPARK_DISTANCE: return 3;
                case ChaseOverlay::PARAM_TRAIL_LENGTH: return 2;
                case ChaseOverlay::PARAM_CHANCE_OF_BOUNCE: return 75; // 0 - 100
                case ChaseOverlay::PARAM_CHANCE_OF_SWIRL: return 0; // 0 - 100
                default: return defaultValue;
            }
        });
    }

    return {};
}

inline LayoutConfig phraseLayoutConfig() {
    return LayoutConfig(
        phraseLayouts,
        {
            {LEDS_IN_LETTERS, "LEDS_IN_LETTERS"},
            {LEDS_IN_WORDS, "LEDS_IN_WORDS"},
            {LEDS_IN_WHOLE, "LEDS_IN_WHOLE"},
            {LETTERS_IN_WORDS, "LETTERS_IN_WORDS"},
            {LETTERS_IN_WHOLE, "LETTERS_IN_WHOLE"},
            {WORDS_IN_WHOLE, "WORDS_IN_WHOLE"},
        },
        phraseLayoutSelector,
        phraseEffectSelector,
        phraseOverlaySelector,
        phraseTransitionSelector,
        phraseParamSelector
    );
}

#endif //PHRASELAYOUTCONFIG_H
