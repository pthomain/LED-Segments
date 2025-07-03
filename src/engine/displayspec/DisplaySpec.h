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

#include "engine/displayspec/LayoutCatalog.h"

class DisplaySpec {
    uint16_t _maxSegmentSize = 0;

    void calculateMaxSegmentSize() {
        for (auto layoutIndex: _catalog.layouts()) {
            for (uint16_t segmentIndex = 0; segmentIndex < nbSegments(layoutIndex); segmentIndex++) {
                _maxSegmentSize = max(_maxSegmentSize, segmentSize(layoutIndex, segmentIndex));
            }
        }
    }

protected:
    const LayoutCatalog _catalog;

public:
    explicit DisplaySpec(const LayoutCatalog &catalog): _catalog(catalog) {
    }

    virtual uint16_t nbLeds() const = 0;

    virtual bool isCircular() const { return false; }

    uint16_t maxSegmentSize() const {
        if (_maxSegmentSize == 0) const_cast<DisplaySpec *>(this)->calculateMaxSegmentSize();
        return _maxSegmentSize;
    };

    virtual uint16_t nbSegments(uint16_t layoutIndex) const = 0;

    virtual uint16_t segmentSize(uint16_t layoutIndex, uint16_t segmentIndex) const = 0;

    virtual void mapLeds(
        uint16_t layoutIndex,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const = 0;

    const LayoutCatalog &catalog() const {
        return _catalog;
    }

    virtual ~DisplaySpec() = default;
};

#endif //LED_SEGMENTS_DISPLAYSPEC_H
