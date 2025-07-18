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

#ifndef LED_SEGMENTS_FIBONACCIDISPLAY_H
#define LED_SEGMENTS_FIBONACCIDISPLAY_H

#include "FibonacciLayoutConfig.h"
#include "engine/displayspec/DisplaySpec.h"

class FibonacciSpec : public DisplaySpec {
    void mapPixel(
        uint16_t variation,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        uint8_t inflexionPoint,
        const std::function<void(uint16_t)> &onLedMapped
    ) const;

    void mapPixelUnit(
        uint16_t variation,
        uint16_t pixelUnitIndex,
        uint8_t inflexionPoint,
        const std::function<void(uint16_t)> &onLedMapped
    ) const;

    WeightedLayout radialToSpiralIndex(
        uint8_t segmentIndex,
        uint8_t radialIndex
    ) const;

    uint8_t getLedOffset(uint8_t pixelIndex) const;

    uint8_t getLedPadding(uint8_t pixelIndex) const;

public :

    static constexpr int LED_PIN = 9;
    static constexpr EOrder RGB_ORDER = GRB;

    explicit FibonacciSpec(): DisplaySpec(fibonacciLayoutCatalog()) {
    }

    uint16_t nbLeds() const override { return TOTAL_FIBONACCI_LEDS; }

    uint16_t nbSegments(uint16_t layoutId) const override;

    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override;

    void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const override;

    ~FibonacciSpec() override = default;
};

#endif //LED_SEGMENTS_FIBONACCIDISPLAY_H
