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

#ifndef LED_SEGMENTS_DISPLAYSPEC_H
#define LED_SEGMENTS_DISPLAYSPEC_H

#include "config/LayoutConfig.h"

namespace LEDSegments {

class DisplaySpec {
    uint16_t _maxSegmentSize = 0;

    void calculateMaxSegmentSize() {
        for (auto layoutId: config.layoutIds) {
            for (uint16_t segmentIndex = 0; segmentIndex < nbSegments(layoutId); segmentIndex++) {
                _maxSegmentSize = max(_maxSegmentSize, segmentSize(layoutId, segmentIndex));
            }
        }
    }

public:
    const LayoutConfig config;
    const uint8_t brightness;
    const uint8_t minEffectDurationsInSecs;
    const uint8_t maxEffectDurationsInSecs;
    const uint8_t fps;
    const int16_t transitionDurationInMillis;
    const uint16_t refreshRateInMillis;
    const float chanceOfRainbow;
    const uint8_t isCircular;

    explicit DisplaySpec(
        const LayoutConfig &config,
        const uint8_t brightness = 50,
        const uint8_t minEffectDurationsInSecs = 3,
        const uint8_t maxEffectDurationsInSecs = 10,
        const int16_t transitionDurationInMillis = 1000, //use < 1 to disable
        const float chanceOfRainbow = .75f,
        const uint8_t fps = 30,
        const uint8_t isCircular = false
    ): config(config),
       brightness(brightness),
       minEffectDurationsInSecs(min(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
       maxEffectDurationsInSecs(max(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
       transitionDurationInMillis(transitionDurationInMillis),
       refreshRateInMillis(1000 / max(1, fps)),
       chanceOfRainbow(chanceOfRainbow),
       fps(max(1, fps)),
       isCircular(isCircular) {}

    virtual uint16_t nbLeds() const = 0;

    uint16_t maxSegmentSize() const {
        if (_maxSegmentSize == 0) const_cast<DisplaySpec *>(this)->calculateMaxSegmentSize();
        return _maxSegmentSize;
    };

    virtual uint16_t nbSegments(uint16_t layoutId) const = 0;

    virtual uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const = 0;

    virtual void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const = 0;

    virtual ~DisplaySpec() = default;
};

} // namespace LEDSegments

#endif //LED_SEGMENTS_DISPLAYSPEC_H
