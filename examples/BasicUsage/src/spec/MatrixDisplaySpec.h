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

#ifndef MATRIXDISPLAYSPEC_H
#define MATRIXDISPLAYSPEC_H

#include "config/EffectConfig.h"
#include "config/LayoutConfig.h"
#include "config/OverlayConfig.h"
#include "config/ParamConfig.h"
#include "config/TransitionConfig.h"
#include "engine/displayspec/DisplaySpec.h"
#include "engine/utils/Utils.h"

// This class defines the display specification (in this case a 8x32 WS2812B LED matrix display).
class MatrixDisplaySpec : public DisplaySpec {
public:
    //The 2 fields below are mandatory
    static constexpr int LED_PIN = 9; // Output pin where the LED strip is connected
    static constexpr EOrder RGB_ORDER = GRB; // Color order of the LED strip (see FastLED documentation)

    explicit MatrixDisplaySpec(): DisplaySpec(
        LayoutConfig(
            layoutIds,
            layoutNames,
            layoutSelector,
            effectSelector,
            overlaySelector,
            transitionSelector,
            paramSelector
        ),
        50, //brightness
        5,  //min effect duration
        5   //max effect duration
    ) {
    }

    // Total number of LEDs in the strip
    uint16_t nbLeds() const override { return TOTAL_LEDS; }

    // Number of segments in the given layout
    uint16_t nbSegments(uint16_t layoutId) const override;

    // Size of the segment at the given index in the given layout
    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override;

    // This method maps the given layout, segment and pixel index to the corresponding LED index on the strip
    void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        fract16 progress,
        // Callback function to call for each mapped LED with its index as a parameter
        const std::function<void(uint16_t)> &onLedMapped
    ) const override;

    ~MatrixDisplaySpec() override = default;
};

#endif // MATRIXDISPLAYSPEC_H
