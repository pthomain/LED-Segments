//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

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

#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include "engine/effect/Effect.h"
#include "engine/displayspec/DisplaySpec.h"
#include "engine/render/Renderer.h"

class Display {
    const uint8_t minEffectDurationsInSecs;
    const uint8_t maxEffectDurationsInSecs;
    uint8_t currentEffectDurationsInSecs;
    const uint8_t fps;
    const int16_t transitionDurationInMillis;
    const uint16_t refreshRateInMillis;
    const DisplaySpec &displaySpec;
    const std::unique_ptr<Renderer> renderer;
    CRGB *outputArray;
    const uint8_t *freePinsForEntropy;
    const uint8_t nbPinsForEntropy;
    uint16_t effectIndex = 0;

    explicit Display(
        CRGB *outputArray,
        const DisplaySpec &displaySpec,
        const uint8_t brightness,
        const uint8_t minEffectDurationsInSecs,
        const uint8_t maxEffectDurationsInSecs,
        const int16_t transitionDurationInMillis,
        const uint8_t fps,
        const uint8_t *freePinsForEntropy,
        const uint8_t nbPinsForEntropy
    );

    void changeEffect();

    void render() const;

public:
    template<int LED_PIN, EOrder RGB_ORDER>
    static Display *create(
        const DisplaySpec &displaySpec,
        const uint8_t brightness = 50,
        const uint8_t minEffectDurationsInSecs = 3,
        const uint8_t maxEffectDurationsInSecs = 10,
        const int16_t transitionDurationInMillis = 1000, //use < 1 to disable
        const uint8_t fps = 30,
        const uint8_t *freePinsForEntropy = new uint8_t[6]{1, 2, 3, 4, 5, 6}, //change if any of those pins are in use
        const uint8_t nbPinsForEntropy = 6
    ) {
        CRGB *outputArray = new CRGB[displaySpec.nbLeds()];
        CFastLED::addLeds<WS2812B, LED_PIN, RGB_ORDER>(outputArray, displaySpec.nbLeds());
        return new Display(
            outputArray,
            std::move(displaySpec),
            brightness,
            minEffectDurationsInSecs,
            maxEffectDurationsInSecs,
            transitionDurationInMillis,
            fps,
            freePinsForEntropy,
            nbPinsForEntropy
        );
    }

    void loop();

    ~Display() {
        delete[] outputArray;
        delete[] freePinsForEntropy;
    }
};

#endif //LED_SEGMENTS_DISPLAY_H
