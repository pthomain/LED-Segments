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

#include "FibonacciSpec.h"
#include "config/FibonacciLayoutConfig.h"
#include "engine/utils/Utils.h"

uint16_t FibonacciSpec::nbSegments(const uint16_t layoutIndex) const {
    //same effect applied to all segments, pixels are used as an optimisation
    //since all segments have the same size (might change later)
    //direction, inflexion and alignment are not meaningful when using PIXEL with this optimisation
    //so they should be filtered out, unless I decide to add a different effect per segment
    return 1;
}

uint16_t FibonacciSpec::nbPixels(const uint16_t layoutIndex, const uint16_t segmentIndex) const {
    const auto variation = variations()[layoutIndex];
    const auto pixelUnit = getPixelUnit(variation);
    const auto alignment = getAlignment(variation);

    if (alignment == SPIRAL) {
        return pixelUnit == PIXEL ? NB_SPIRAL_PIXELS : NB_SPIRAL_SEGMENTS;
    } else {
        //different effect per segment, otherwise the whole display would have a single colour
        return pixelUnit == PIXEL ? NB_RADIAL_PIXELS : NB_RADIAL_SEGMENTS;
    }
}

std::pair<uint16_t, uint8_t> FibonacciSpec::radialToSpiralIndex(
    const uint8_t segmentIndex,
    const uint8_t radialIndex
) const {
    const bool isEvenSegment = segmentIndex % 2 == 0;
    const uint8_t segmentOffset = isEvenSegment ? segmentIndex / 2 : (segmentIndex + 1) / 2;
    const uint16_t segmentStart = unsignedModulo(NB_LEDS_IN_SPIRAL * (segmentOffset + radialIndex), nbLeds());
    uint8_t spiralPixelIndex;

    if (isEvenSegment) {
        spiralPixelIndex = radialIndex * 2;
    } else {
        spiralPixelIndex = (radialIndex * 2) + 1;
    }

    return std::pair(segmentStart, spiralPixelIndex);
}

uint8_t FibonacciSpec::getLedOffset(const uint8_t spiralPixelIndex) const {
    switch (spiralPixelIndex) {
        case 16:
            return 1;
        case 17:
            return 2;
        case 18:
            return 3;
        case 19:
            return 5;
        default:
            return 0;
    }
}

uint8_t FibonacciSpec::getLedPadding(const uint8_t spiralPixelIndex) const {
    switch (spiralPixelIndex) {
        case 15:
        case 16:
        case 17:
            return 1;
        case 18:
        case 19:
            return 2;
        default:
            return 0;
    }
}

void FibonacciSpec::mapPixel(
    const uint16_t variation,
    const uint16_t segmentIndex,
    const uint16_t pixelIndex,
    const uint8_t inflexionPoint,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    const auto mapLed = [&](uint16_t ledIndex) {
        //segments 0 and 1 are swapped
        if (ledIndex < 27) {
            onLedMapped(ledIndex + 27);
        } else if (ledIndex >= 27 && ledIndex < 54) {
            onLedMapped(ledIndex - 27);
        } else {
            onLedMapped(ledIndex);
        }
    };

    const auto direction = getDirection(variation);
    const auto alignment = getAlignment(variation);

    uint16_t segmentStart;
    uint8_t spiralPixelIndex;
    if (alignment == RADIAL) {
        const auto segmentStartAndSpiralLedIndex = radialToSpiralIndex(segmentIndex, pixelIndex);
        segmentStart = segmentStartAndSpiralLedIndex.first;
        spiralPixelIndex = segmentStartAndSpiralLedIndex.second;
    } else {
        const auto ccwOffset = unsignedModulo(
            segmentIndex + pixelIndex - inflexionPoint,
            NB_SPIRAL_SEGMENTS
        );

        uint8_t directedSegmentIndex;
        //TODO special case with no inflexion, at the moment simple spirals are reversed
        //TODO check why travel is skipping every other segment

        if (pixelIndex > inflexionPoint) {
            directedSegmentIndex = direction == CLOCKWISE ? segmentIndex : ccwOffset;
        } else {
            directedSegmentIndex = direction == CLOCKWISE ? ccwOffset : segmentIndex;
        }

        spiralPixelIndex = pixelIndex;
        segmentStart = unsignedModulo(
            NB_LEDS_IN_SPIRAL * directedSegmentIndex,
            nbLeds()
        );
    }

    uint16_t ledIndex = segmentStart + spiralPixelIndex + getLedOffset(spiralPixelIndex);

    for (uint8_t padding = 0; padding <= getLedPadding(spiralPixelIndex); padding++) {
        mapLed(ledIndex + padding);
    }
};

void FibonacciSpec::mapPixelUnit(
    const uint16_t variation,
    const uint16_t pixelUnitIndex,
    const uint8_t inflexionPoint,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    if (getPixelUnit(variation) == PIXEL) {
        //the colour must be applied to the same pixel index for each segment
        uint16_t nbSegments = getAlignment(variation) == SPIRAL ? NB_SPIRAL_SEGMENTS : NB_RADIAL_SEGMENTS;
        for (uint16_t segmentIndex = 0; segmentIndex < nbSegments; segmentIndex++) {
            mapPixel(
                variation,
                segmentIndex,
                pixelUnitIndex, //pixel index
                inflexionPoint,
                onLedMapped
            );
        }
    } else {
        //each pixel index represents a segment and the colour must be applied to the entire segment
        uint16_t nbPixels = getAlignment(variation) == SPIRAL ? NB_SPIRAL_PIXELS : NB_RADIAL_PIXELS;
        for (uint16_t pixelIndex = 0; pixelIndex < nbPixels; pixelIndex++) {
            mapPixel(
                variation,
                pixelUnitIndex, //segment index
                pixelIndex,
                inflexionPoint,
                onLedMapped
            );
        }
    }
};

void FibonacciSpec::mapLeds(
    uint16_t layoutIndex,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    float progress,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    const auto variation = variations()[layoutIndex];
    const auto inflexion = getInflexion(variation);
    uint8_t inflexionPoint;

    switch (inflexion) {
        case STATIC_INFLEXION:
            inflexionPoint = 15;
            break;
        case DYNAMIC_INFLEXION:
            inflexionPoint = static_cast<uint8_t>(progress * NB_SPIRAL_PIXELS);
            break;
        case INFLEXION_NONE:
        default:
            inflexionPoint = -1;
            break;
    }

    mapPixelUnit(
        variation,
        pixelIndex, //optimisation for spirals, each segment is considered a pixel
        inflexionPoint,
        onLedMapped
    );
}
