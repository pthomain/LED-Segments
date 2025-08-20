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

#include "ChaosEffect.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {
    static const ChaosEffectFactory factoryInstance;
    RenderableFactoryRef<CRGB> ChaosEffect::factory = &factoryInstance;

    const uint8_t ChaosEffect::PARAM_PALETTE_INCREMENT;

    ChaosEffect::ChaosEffect(const RenderableContext &context)
        : Effect(context),
          paletteIncrement(param(PARAM_PALETTE_INCREMENT)),
          segmentRotationSpeeds(std::make_unique<uint8_t[]>(context.nbSegments)),
          segmentDirections(std::make_unique<uint8_t[]>(context.nbSegments)),
          segmentInitialPositions(std::make_unique<uint8_t[]>(context.nbSegments)) {
        // Generate unique chaotic parameters for each segment
        for (uint16_t segmentIndex = 0; segmentIndex < context.nbSegments; segmentIndex++) {
            // Use segment index as seed for deterministic but chaotic values
            segmentRotationSpeeds[segmentIndex] = random8(20, 80);
            segmentDirections[segmentIndex] = random8(2); // 0 or 1
            segmentInitialPositions[segmentIndex] = random8();
        }
    }

    void ChaosEffect::fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentArraySize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) {
        // Use progress for animation progression (fract16 0-65535 maps to 0-255)
        uint8_t progressValue = progress >> 8;

        // Get segment-specific parameters from arrays
        uint8_t actualRotationSpeed = segmentRotationSpeeds[segmentIndex];
        uint8_t actualDirection = segmentDirections[segmentIndex];
        uint8_t actualInitialPosition = segmentInitialPositions[segmentIndex];

        // Calculate time-based rotation with direction and progress
        uint8_t rotationOffset;
        if (actualDirection) {
            rotationOffset = ((timeInMillis * actualRotationSpeed) >> 8) + (progressValue >> 2);
        } else {
            rotationOffset = 255 - ((timeInMillis * actualRotationSpeed) >> 8) - (progressValue >> 2);
        }

        // Calculate segment's current position in HSV/palette space
        uint8_t segmentBasePosition = actualInitialPosition + rotationOffset;

        uint8_t increment = max(1, 255 / segmentArraySize * paletteIncrement);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentArraySize; pixelIndex++) {
            // Calculate palette index for this pixel with increment
            uint8_t paletteIndex = segmentBasePosition + (pixelIndex * increment);

            // Get color from context palette with full brightness
            CRGB paletteColor = ColorFromPalette(
                context.palette.palette,
                paletteIndex,
                255,
                LINEARBLEND
            );
            segmentArray[pixelIndex] = paletteColor;
        }
    }
} // LEDSegments
