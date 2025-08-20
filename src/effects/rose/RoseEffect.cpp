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

#include "RoseEffect.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {
    static const RoseEffectFactory factoryInstance;
    RenderableFactoryRef<CRGB> RoseEffect::factory = &factoryInstance;

    RoseEffect::RoseEffect(const RenderableContext &context)
        : Effect(context),
          rotationSpeed(param(PARAM_ROTATION_SPEED)),
          hueSpeed(param(PARAM_HUE_SPEED)),
          roseNumerator(param(PARAM_N_NUMERATOR)),
          roseDenominator(param(PARAM_D_DENOMINATOR)) {
    }

    void RoseEffect::calculateRosePoint(uint8_t angle, uint8_t &x, uint8_t &y) {
        // Maurer rose equations optimized for 8-bit arithmetic:
        // r = sin(n/d * theta)
        // x = r * cos(theta)
        // y = r * sin(theta)

        // Calculate n/d * theta using 8-bit math
        uint16_t nTheta = ((uint16_t) roseNumerator * angle) / roseDenominator;
        uint8_t rAngle = (uint8_t) (nTheta & 0xFF);

        // Get radius using sin8 (FastLED's optimized sine)
        uint8_t radius = sin8(rAngle);

        // Use maximum scale for full rose size
        // radius already at full scale from sin8 (no scaling needed)

        // Calculate x = radius * cos(angle), y = radius * sin(angle)
        int16_t cosValue = cos8(angle) - 128; // Convert to signed (-128 to +127)
        int16_t sinValue = sin8(angle) - 128;

        // Apply radius and convert back to unsigned coordinates centered at 128
        x = 128 + ((cosValue * radius) >> 8);
        y = 128 + ((sinValue * radius) >> 8);
    }

    void RoseEffect::fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentArraySize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) {
        // Segment-specific offsets for unique variations per segment
        uint8_t segmentOffset = segmentIndex * 29; // Prime number for good distribution
        uint8_t segmentHueShift = segmentIndex * 17; // Different prime for hue variation

        // Use progress for animation progression (fract16 0-65535 maps to 0-255)
        uint8_t progressValue = progress >> 8;
        
        // Pre-calculate time-based animation values using const members
        uint8_t rotationOffset = ((timeInMillis * rotationSpeed) >> 8) + segmentOffset + (progressValue >> 2);
        uint8_t baseHueTime = ((timeInMillis * hueSpeed) >> 8) + segmentHueShift + (progressValue >> 3);

        for (uint16_t pixelIndex = 0; pixelIndex < segmentArraySize; pixelIndex++) {
            // Map pixel position to angle on the rose curve
            uint8_t angle = ((pixelIndex * 255) / segmentArraySize) + rotationOffset;

            // Calculate rose coordinates
            uint8_t roseX, roseY;
            calculateRosePoint(angle, roseX, roseY);

            // Map rose coordinates to palette index for full spectrum coverage
            // Use rose coordinates directly for spatial palette variation
            uint8_t spatialIndex = (roseX + roseY) >> 1; // Spatial contribution (0-255 range)
            
            // Add angle contribution for curve-based color progression  
            uint8_t angleIndex = angle; // Full angle range (0-255)
            
            // Add time animation for color cycling
            uint8_t timeIndex = baseHueTime; // Full time range (0-255)
            
            // Add progress influence for dynamic palette progression
            uint8_t progressInfluence = progressValue >> 1; // Progress contribution
            
            // Create palette index ensuring full spectrum usage with natural overflow
            uint8_t paletteIndex = spatialIndex + (angleIndex >> 1) + (timeIndex >> 2) + (progressInfluence >> 2);

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
