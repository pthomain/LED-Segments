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

#include "HatSpec.h"
#include <vector>
#include <map>

static const auto *concentric = new std::vector<std::vector<uint8_t> >{
    {18},
    {17, 11, 12, 19, 25, 24},
    {16, 10, 5, 6, 7, 13, 20, 26, 31, 30, 29, 23},
    {15, 9, 4, 0, 1, 2, 3, 8, 14, 21, 27, 32, 36, 35, 34, 33, 28, 22}
};

static const auto *rows = new std::vector<std::vector<uint8_t> >{
    {0, 1, 2, 3},
    {4, 5, 6, 7, 8},
    {9, 10, 11, 12, 13, 14},
    {15, 16, 17, 18, 19, 20, 21},
    {22, 23, 24, 25, 26, 27},
    {28, 29, 30, 31, 32},
    {33, 34, 35, 36}
};

static const auto *columns = new std::vector<std::vector<uint8_t> >{
    {9, 15, 22, 0, 4, 10, 16, 23, 28, 33},
    {1, 5, 11, 17, 24, 29, 34},
    {2, 6, 12, 18, 25, 30, 35},
    {3, 7, 13, 19, 26, 31, 36},
    {8, 14, 20, 27, 32, 21}
};

static const auto *diagonalTopLeft = new std::vector<std::vector<uint8_t> >{
    {0, 4, 9, 15},
    {1, 5, 10, 16, 22},
    {2, 6, 11, 17, 23, 28},
    {3, 7, 12, 18, 24, 29, 33},
    {8, 13, 19, 25, 30, 34},
    {14, 20, 26, 31, 35},
    {21, 27, 32, 36},
};

static const auto *diagonalTopRight = new std::vector<std::vector<uint8_t> >{
    {3, 8, 14, 21},
    {2, 7, 13, 20, 27},
    {1, 6, 12, 19, 26, 32},
    {0, 5, 11, 18, 25, 31, 36},
    {4, 10, 17, 24, 30, 35},
    {9, 16, 23, 29, 34},
    {15, 22, 28, 33}
};

static const auto *diagonalBottomLeft = new std::vector<std::vector<uint8_t> >{
    {15, 22, 28, 33},
    {9, 16, 23, 29, 34},
    {4, 10, 17, 24, 30, 35},
    {0, 5, 11, 18, 25, 31, 36},
    {1, 6, 12, 19, 26, 32},
    {2, 7, 13, 20, 27},
    {3, 8, 14, 21}
};

static const auto *diagonalBottomRight = new std::vector<std::vector<uint8_t> >{
    {21, 27, 32, 36},
    {14, 20, 26, 31, 35},
    {8, 13, 19, 25, 30, 34},
    {3, 7, 12, 18, 24, 29, 33},
    {2, 6, 11, 17, 23, 28},
    {1, 5, 10, 16, 22},
    {0, 4, 9, 15}
};

static const auto *spiral = new std::vector<std::vector<uint8_t> >{
    {25, 26, 31, 32, 35, 36},
    {19, 13, 20, 14, 27, 21, 18},
    {8, 3, 2, 7, 2, 12, 6},
    {1, 0, 5, 4, 11, 10, 9},
    {15, 16, 22, 23, 17},
    {24, 28, 29, 30, 33, 34},
};

static const auto *radial = new std::vector<std::vector<uint8_t> >{
    {15, 16, 17, 18},
    {9, 4, 10},
    {0, 5, 11},
    {1, 2, 6},
    {3, 7, 12},
    {8, 14, 13},
    {19, 20, 21},
    {26, 27, 32},
    {25, 31, 36},
    {30, 34, 35},
    {24, 29, 33},
    {22, 23, 28},
};

static const std::vector<uint8_t> emptyVector = {};

const std::vector<uint8_t> &HatSpec::eyeSegment(uint16_t layoutId, uint16_t pixelIndex) {
    auto &pixels = concentric;

    switch (layoutId) {
        case EYE_CONCENTRIC: pixels = concentric;
            break;
        case EYE_SPIRAL: pixels = spiral;
            break;
        case EYE_ROW: pixels = rows;
            break;
        case EYE_COLUMN: pixels = columns;
            break;
        case EYE_RADIAL: pixels = radial;
            break;
        case EYE_DIAGONAL_TOP_RIGHT: pixels = diagonalTopRight;
            break;
        case EYE_DIAGONAL_TOP_LEFT: pixels = diagonalTopLeft;
            break;
        case EYE_DIAGONAL_BOTTOM_RIGHT: pixels = diagonalBottomRight;
            break;
        case EYE_DIAGONAL_BOTTOM_LEFT: pixels = diagonalBottomLeft;
            break;
    }

    if (pixelIndex < pixels->size()) {
        return pixels->at(pixelIndex);
    }

    return emptyVector;
}

uint16_t HatSpec::nbSegments(uint16_t layoutId) const {
    return 3; //eyes are rendered independently for mirrors + all panels duplicated
}

uint16_t HatSpec::segmentSize(uint16_t layoutId, uint16_t segmentIndex) const {
    if (segmentIndex == 0) {
        switch (layoutId) {
            case EYE_CONCENTRIC: return concentric->size();

            case EYE_SPIRAL: return spiral->size();

            case EYE_RADIAL: return radial->size();

            case EYE_ROW: return rows->size();

            case EYE_COLUMN: return columns->size();

            case EYE_DIAGONAL_TOP_RIGHT:
                return diagonalTopRight->size();

            case EYE_DIAGONAL_TOP_LEFT:
                return diagonalTopLeft->size();

            case EYE_DIAGONAL_BOTTOM_RIGHT:
                return diagonalBottomRight->size();

            case EYE_DIAGONAL_BOTTOM_LEFT:
                return diagonalBottomLeft->size();

            case EYE_LINEAR:
            default: return LEDS_PER_EYE;
        }
    }
    return LEDS_PER_PANEL;
}

void HatSpec::mapLeds(
    uint16_t layoutId,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    float progress,
    const std::function<void(uint16_t)> &onLedMapped
) const {
    if (segmentIndex < 2) {
        uint8_t prefix = NB_PANELS * LEDS_PER_PANEL;
        if (layoutId == EYE_LINEAR) {
            onLedMapped(prefix + pixelIndex);
            onLedMapped(prefix + pixelIndex + LEDS_PER_EYE);
        } else {
            for (const auto &ledIndex: eyeSegment(layoutId, pixelIndex)) {
                onLedMapped(prefix + ledIndex);
                //mirroring the other eye
                onLedMapped(prefix + (LEDS_PER_EYE - ledIndex-1) + LEDS_PER_EYE);
            }
        }
    } else {
        for (uint8_t i = 0; i < NB_PANELS; i++) {
            onLedMapped(i * LEDS_PER_PANEL + pixelIndex);
        }
    }
}
