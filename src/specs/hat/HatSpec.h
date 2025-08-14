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

#ifndef HATSPEC_H
#define HATSPEC_H

#include "config/HatEffectConfig.h"
#include "config/HatLayoutConfig.h"
#include "config/HatOverlayConfig.h"
#include "config/HatParamConfig.h"
#include "config/HatTransitionConfig.h"
#include "engine/displayspec/DisplaySpec.h"
#include "engine/utils/Utils.h"
#include "overlays/CompositeOverlay.h"

class HatSpec : public DisplaySpec {
    CompositeOverlayFactory overlayFactory = CompositeOverlayFactory();
    EffectFactoryRef<CRGB> compositeFactoryRef = &overlayFactory;

    const LayoutConfig wrappedConfig;

    static const std::vector<uint8_t> &eyeCircle(uint16_t pixelIndex);

public:
    static constexpr int LED_PIN = IS_DEBUG ? 9 : 5;
    static constexpr EOrder RGB_ORDER = GRB;

    explicit HatSpec()
        : DisplaySpec(
              LayoutConfig(
                  hatLayoutIds,
                  hatLayoutNames,
                  hatLayoutSelector,
                  hatEffectSelector,
                  [&](uint16_t _) {
                      return EffectAndMirrors<CRGB>{
                          just(compositeFactoryRef),
                          noMirrors<CRGB>
                      };
                  },
                  hatTransitionSelector,
                  hatParamSelector
              ),
              255,
              IS_DEBUG ? 3 : 3,
              IS_DEBUG ? 3 : 8,
              1000,
              0.5f
          ),
          wrappedConfig(
              LayoutConfig(
                  hatLayoutIds,
                  hatLayoutNames,
                  hatLayoutSelector,
                  hatEffectSelector,
                  hatOverlaySelector,
                  hatTransitionSelector,
                  hatParamSelector
              )
          ) {
        overlayFactory.setConfig(&wrappedConfig);
    }

    uint16_t nbLeds() const override { return (LEDS_PER_EYE * 2) + (NB_PANELS * LEDS_PER_PANEL); }

    uint16_t nbSegments(uint16_t layoutId) const override;

    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override;

    void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const override;

    ~HatSpec() override = default;
};

#endif //HATSPEC_H
