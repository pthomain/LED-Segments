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

#ifndef LED_SEGMENTS_FIBONACCIENUMS_H
#define LED_SEGMENTS_FIBONACCIENUMS_H

#include <vector>
#include "Arduino.h"
#include "WString.h"
#include "engine/utils/Utils.h"
#include <effects/noise/NoiseEffect.h>
#include <effects/gradient/GradientEffect.h>
#include <effects/swirl/SwirlEffect.h>
#include <effects/slide/SlideEffect.h>
#include <engine/displayspec/LayoutCatalog.h>
#include "engine/transitions/Transition.h"
#include "overlays/dash/DashOverlay.h"
#include "overlays/moire/MoireOverlay.h"
#include "overlays/chase/ChaseOverlay.h"

const uint16_t TOTAL_FIBONACCI_LEDS = 324;
const uint8_t NB_LEDS_IN_SPIRAL = 27;
const uint8_t NB_SPIRAL_PIXELS = 20;
const uint8_t NB_RADIAL_PIXELS = 10;
const uint8_t NB_SPIRAL_SEGMENTS = 12;
const uint8_t NB_RADIAL_SEGMENTS = 24;

const uint8_t PIXEL_UNIT_MASK = 0b00000001;
const uint8_t DIRECTION_MASK = 0b00000010;
const uint8_t ALIGNMENT_MASK = 0b00000100;
const uint8_t INFLEXION_MASK = 0b00011000;

enum PixelUnit {
    PIXEL,
    SEGMENT
};

enum Direction {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

enum Alignment {
    SPIRAL,
    RADIAL
};

enum Inflexion {
    INFLEXION_NONE,
    STATIC_INFLEXION,
    DYNAMIC_INFLEXION
};

static PixelUnit getPixelUnit(uint8_t layout) {
    return (layout & PIXEL_UNIT_MASK) ? SEGMENT : PIXEL;
}

static Direction getDirection(uint8_t layout) {
    return (layout & DIRECTION_MASK) ? COUNTER_CLOCKWISE : CLOCKWISE;
}

static Alignment getAlignment(uint8_t layout) {
    return (layout & ALIGNMENT_MASK) ? RADIAL : SPIRAL;
}

static Inflexion getInflexion(uint8_t layout) {
    return static_cast<Inflexion>((layout & INFLEXION_MASK) >> 3);
}

static uint8_t getLayout(
    PixelUnit pixelUnit,
    Direction direction,
    Alignment alignment,
    Inflexion inflexion
) {
    uint8_t pixelUnitValue = (pixelUnit == PIXEL ? 0 : 1);
    uint8_t directionValue = (direction == CLOCKWISE ? 0 : 1) << 1;
    uint8_t alignmentValue = (alignment == SPIRAL ? 0 : 1) << 2;
    uint8_t inflexionValue = static_cast<uint8_t>(inflexion) << 3;

    return inflexionValue | alignmentValue | directionValue | pixelUnitValue;
}

static String getLayoutName(const uint16_t variation) {
    auto pixelUnit = getPixelUnit(variation);
    auto direction = getDirection(variation);
    auto alignment = getAlignment(variation);
    auto inflexion = getInflexion(variation);

    String alignmentName = alignment == SPIRAL ? "_SPIRAL" : "_RADIAL";
    String inflexionName;

    switch (inflexion) {
        case STATIC_INFLEXION:
            inflexionName = "_STAT_INF";
            break;
        case DYNAMIC_INFLEXION:
            inflexionName = "_DYNA_INF";
            break;
        case INFLEXION_NONE:
        default:
            inflexionName = "";
            break;
    }

    String directionName = pixelUnit == PIXEL || alignment == RADIAL ? "" : direction == CLOCKWISE ? "_CW" : "_CCW";

    return String(pixelUnit == PIXEL ? "PIXEL" : "SEGMENT") + alignmentName + inflexionName + directionName;
}

static std::vector<uint16_t> computeVariations() {
    auto variations = std::vector<uint16_t>();

    auto addVariation = [&](
        uint8_t pixelUnit,
        uint8_t direction,
        uint8_t alignment,
        uint8_t inflexion
    ) {
        variations.push_back(getLayout(
            static_cast<PixelUnit>(pixelUnit),
            static_cast<Direction>(direction),
            static_cast<Alignment>(alignment),
            static_cast<Inflexion>(inflexion)
        ));
        if constexpr (IS_DEBUG) {
            uint8_t lastIndex = variations.size() - 1;
            Serial.println(String(lastIndex) + ": " + String(getLayoutName(variations.at(lastIndex))));
        }
    };

    //Spiral is very similar to radial for PIXEL, but has a smoother gradient so radial is omitted
    //Direction also doesn't matter to PIXEL in this case
    addVariation(PIXEL, CLOCKWISE, SPIRAL, INFLEXION_NONE);

    //Direction and inflexion don't apply to SEGMENT radial
    addVariation(SEGMENT, CLOCKWISE, RADIAL, INFLEXION_NONE);

    for (uint8_t direction = 0; direction < 2; direction++) {
        for (uint8_t inflexion = 0; inflexion < 3; inflexion++) {
            addVariation(SEGMENT, direction, SPIRAL, inflexion);
        }
    }
    return variations;
}

static EffectAndMirrors<CRGB> fibonacciEffectSelector(uint16_t layoutIndex) {
    return {
        {
            {&GradientEffect::factory, 1},
            {&SwirlEffect::factory, 1},
            {&NoiseEffect::factory, 1},
            {&SlideEffect::factory, 1}
        },
        allCRGBMirrors
    };
}

static EffectAndMirrors<CRGB> fibonacciOverlaySelector(uint16_t layoutIndex) {
    return {
        {
            {&MoireOverlay::factory, 1},
            {&ChaseOverlay::factory, 1},
            {&DashOverlay::factory, 1},
        },
        allCRGBMirrors
    };
}

static EffectAndMirrors<uint8_t> fibonacciTransitionSelector(uint16_t layoutIndex) {
    return ALL_TRANSITIONS;
}

static LayoutCatalog fibonacciLayoutCatalog(const std::vector<uint16_t> &variations) {
    auto names = std::map<uint16_t, String>();
    for (auto variation: variations) {
        names.insert(std::pair(variation, getLayoutName(variation)));
    }
    return LayoutCatalog(
        variations,
        names,
        fibonacciEffectSelector,
        fibonacciOverlaySelector,
        fibonacciTransitionSelector
    );
}

#endif //LED_SEGMENTS_FIBONACCIENUMS_H
