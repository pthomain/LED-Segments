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

#ifndef TEST_TRANSITION_CONFIG_H
#define TEST_TRANSITION_CONFIG_H

#include "engine/displayspec/config/LayoutConfig.h"
#include "transitions/fade/FadeTransition.h"
#include "transitions/slide/SlideTransition.h"

// Returns the transitions to be used for the given layout
static RenderablesAndMirrors<uint8_t> transitionSelector(uint16_t layoutId) {
    switch (layoutId) {
        // case GROUP_BY_4:
            default:
            return {
                {
                    // This is the initializer list for the std::vector
                    // {SlideTransition::factory, 4}, // 4 times more likely to be selected than FadeTransition
                    {FadeTransition::factory, 1}
                },
                allMirrors<uint8_t> //apply any mirror to the selected transitions
            };
        //
        // default:
        //     return {
        //         just(FadeTransition::factory),
        //         MirrorSelector<uint8_t>(noMirrors<uint8_t>) //no mirror applied to the FadeTransition
        //     };
    }
}

#endif // TEST_TRANSITION_CONFIG_H
