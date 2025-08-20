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

#include "FractalEffect.h"
#include "engine/utils/Utils.h"

namespace LEDSegments {

static const FractalEffectFactory factoryInstance;
RenderableFactoryRef<CRGB> FractalEffect::factory = &factoryInstance;

const uint8_t FractalEffect::PARAM_SPEED;
const uint8_t FractalEffect::PARAM_ROUGHNESS;

void FractalEffect::fillSegmentArray(
    CRGB *segmentArray,
    uint16_t segmentArraySize,
    uint16_t segmentIndex,
    fract16 progress,
    unsigned long timeInMillis
) {
    // Segment-specific offsets for unique fractal variations per segment
    uint8_t segmentOffset = segmentIndex * 31; // Prime number for good distribution
    uint8_t segmentHueShift = segmentIndex * 19; // Different prime for hue variation
    uint8_t segmentPhaseShift = segmentIndex * 13; // Another prime for phase differences
    
    // Use progress for animation progression (fract16 0-65535 maps to 0-255)
    uint8_t progressValue = progress >> 8;
    
    // Pre-calculate time-based animation values with segment variations
    uint8_t fractalTimeScale = ((timeInMillis + segmentOffset * 8) >> 5) & 0xFF; // Slower fractal evolution
    uint8_t hueAnimationTime = (timeInMillis + segmentHueShift + (progressValue >> 3)) & 0xFF; // Hue cycling with progress
    uint8_t roughnessTime = ((timeInMillis + segmentPhaseShift * 4) >> 4) & 0xFF; // Roughness variation

    for (uint16_t pixelIndex = 0; pixelIndex < segmentArraySize; pixelIndex++) {
        // Multi-octave noise for fractal-like patterns
        uint8_t baseNoise = normaliseNoise(inoise8(pixelIndex * 4, fractalTimeScale, segmentIndex * 7));
        uint8_t detailNoise = normaliseNoise(inoise8(pixelIndex * 8, roughnessTime, (segmentIndex + 64) * 5)) >> 1;
        uint8_t fineNoise = normaliseNoise(inoise8(pixelIndex * 16, fractalTimeScale >> 1, segmentIndex * 3)) >> 2;
        
        // Combine octaves for fractal-like complexity
        uint8_t fractalValue = baseNoise + detailNoise + fineNoise;
        
        // Multi-layer palette index calculation for full spectrum coverage
        // Fractal spatial contribution
        uint8_t spatialIndex = fractalValue; // Direct fractal contribution (0-255 range)
        
        // Speed-influenced animation
        uint8_t animatedIndex = ((speed * hueAnimationTime) >> 8); // Speed animation
        
        // Roughness influence using triwave for organic variation
        uint8_t roughnessInfluence = (roughness * triwave8(fractalValue + roughnessTime)) >> 8;
        
        // Add progress influence for dynamic palette progression
        uint8_t progressInfluence = progressValue >> 1; // Progress contribution
        
        // Create palette index ensuring full spectrum usage with natural overflow
        uint8_t paletteIndex = spatialIndex + (animatedIndex >> 1) + (roughnessInfluence >> 2) + (progressInfluence >> 2);
        
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