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

#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include "engine/effect/Effect.h"
#include "engine/displayspec/DisplaySpec.h"
#include "engine/render/Renderer.h"
#include <algorithm> // Required for std::remove and std::erase

class Display {
    const uint8_t minEffectDurationsInSecs;
    const uint8_t maxEffectDurationsInSecs;
    uint8_t currentEffectDurationsInSecs;
    uint32_t lastChangeTime = 0;
    const uint8_t fps;
    const int16_t transitionDurationInMillis;
    const uint16_t refreshRateInMillis;
    const DisplaySpec &displaySpec;
    const std::unique_ptr<Renderer> renderer;
    const std::vector<uint8_t> freePinsForEntropy;

    explicit Display(
        CRGB *outputArray,
        const DisplaySpec &displaySpec,
        uint8_t brightness,
        uint8_t minEffectDurationsInSecs,
        uint8_t maxEffectDurationsInSecs,
        int16_t transitionDurationInMillis,
        uint8_t fps,
        const std::vector<uint8_t> &freePinsForEntropy
    );

    void changeEffect(uint8_t effectDurationsInSecs);

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
        //change if any of those pins are already in use or unavailable on the board
        std::vector<unsigned char> freePinsForEntropy = std::vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    ) {
        freePinsForEntropy.erase(
            std::remove(freePinsForEntropy.begin(), freePinsForEntropy.end(), static_cast<uint8_t>(LED_PIN)),
            freePinsForEntropy.end()
        );

        CRGB *outputArray = new CRGB[displaySpec.nbLeds()];
        CFastLED::addLeds<WS2812B, LED_PIN, RGB_ORDER>(outputArray, displaySpec.nbLeds())
                .setCorrection(TypicalLEDStrip);

        return new Display(
            outputArray,
            displaySpec,
            brightness,
            minEffectDurationsInSecs,
            maxEffectDurationsInSecs,
            transitionDurationInMillis,
            fps,
            freePinsForEntropy
        );
    }

    void loop();

    ~Display() = default;
};

#endif //LED_SEGMENTS_DISPLAY_H
