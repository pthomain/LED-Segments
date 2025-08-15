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

#ifndef UMBRELLASPEC_H
#define UMBRELLASPEC_H

#include "config/UmbrellaEffectConfig.h"
#include "config/UmbrellaLayoutConfig.h"
#include "config/UmbrellaOverlayConfig.h"
#include "config/UmbrellaParamConfig.h"
#include "config/UmbrellaTransitionConfig.h"
#include "lib/engine/displayspec/DisplaySpec.h"
#include "lib/engine/utils/Utils.h"

constexpr uint8_t NB_SPOKES = 8;
constexpr uint8_t LEDS_PER_SPOKE = 42;

class UmbrellaSpec : public DisplaySpec {
public:
    static constexpr int LED_PIN = D7;
    static constexpr EOrder RGB_ORDER = GRB;

    explicit UmbrellaSpec(): DisplaySpec(
        LayoutConfig(
            umbrellaLayoutIds,
            umbrellaLayoutNames,
            umbrellaLayoutSelector,
            umbrellaRenderableSelector,
            umbrellaOverlaySelector,
            umbrellaTransitionSelector,
            umbrellaParamSelector
        ),
        IS_DEBUG ? 50 : 128,
        IS_DEBUG ? 10 : 3,
        IS_DEBUG ? 10 : 8,
        1000,
        0.5f
    ) {
    }

    uint16_t nbLeds() const override { return NB_SPOKES * LEDS_PER_SPOKE; }

    uint16_t nbSegments(uint16_t layoutId) const override;

    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override;

    void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const override;

    ~UmbrellaSpec() override = default;
};

#endif //UMBRELLASPEC_H
