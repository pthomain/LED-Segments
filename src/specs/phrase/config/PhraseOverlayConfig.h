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

#ifndef PHRASE_OVERLAY_CONFIG_H
#define PHRASE_OVERLAY_CONFIG_H

#include "engine/displayspec/config/LayoutConfig.h"
#include "PhraseLayoutConfig.h"
#include "engine/effect/Effect.h"
#include "overlays/none/NoOverlay.h"
#include "engine/utils/Weights.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"
#include "overlays/wave/WaveOverlay.h"

static RenderablesAndMirrors<CRGB> phraseOverlaySelector(uint16_t layoutId) {
    switch (layoutId) {
        case LEDS_IN_LETTERS:
            return RenderablesAndMirrors<CRGB>{
                {
                    {WaveOverlay::factory, 1},
                    {NoOverlay::factory, 2},
                },
                undividedMirrors<CRGB>
            };

        case LEDS_IN_WORDS:
            return RenderablesAndMirrors<CRGB>{
                {
                    {WaveOverlay::factory, 2},
                    {DashOverlay::factory, 1},
                    {NoOverlay::factory, 4},
                },
                [](RenderableFactoryRef<CRGB> overlayFactory) {
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
            return RenderablesAndMirrors<CRGB>{
                {
                    {ChaseOverlay::factory, 1},
                    {DashOverlay::factory, 1},
                    {NoOverlay::factory, 6},
                },
                [](RenderableFactoryRef<CRGB> overlayFactory) {
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

#endif //PHRASE_OVERLAY_CONFIG_H