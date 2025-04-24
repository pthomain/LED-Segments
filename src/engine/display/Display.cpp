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

#include "Display.h"
#include "engine/render/simple/SimpleRenderer.h"
#include "engine/render/blender/Blender.h"
#include "utils/SeedGenerator.h"
#include "engine/render/simple/SimplePixelMapper.h"
#include <FastLED.h>

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
    CRGB *outputArray,
    const DisplaySpec &displaySpec,
    const uint8_t brightness,
    const uint8_t minEffectDurationsInSecs,
    const uint8_t maxEffectDurationsInSecs,
    const int16_t transitionDurationInMillis,
    const uint8_t fps,
    const uint8_t *freePinsForEntropy,
    const uint8_t nbPinsForEntropy
) : minEffectDurationsInSecs(minEffectDurationsInSecs),
    maxEffectDurationsInSecs(maxEffectDurationsInSecs),
    currentEffectDurationsInSecs(random8(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
    fps(fps),
    transitionDurationInMillis(transitionDurationInMillis),
    refreshRateInMillis(1000 / fps),
    displaySpec(std::move(displaySpec)),
    renderer(std::unique_ptr<Renderer>(
        transitionDurationInMillis < 1
            ? (Renderer *) new SimpleRenderer(displaySpec, (PixelMapper *) new SimplePixelMapper(displaySpec), "simple")
            : new Blender(displaySpec, "blender", refreshRateInMillis, transitionDurationInMillis)
    )),
    outputArray(outputArray),
    freePinsForEntropy(freePinsForEntropy),
    nbPinsForEntropy(nbPinsForEntropy) {
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    addEntropy(freePinsForEntropy, nbPinsForEntropy);
    changeEffect();
    render();
}

void Display::changeEffect() {
    const auto catalog = displaySpec.catalog();
    const auto effects = catalog.supportedEffects();

    if (effects.empty()) {
        if constexpr (IS_DEBUG) Serial.print("No effects provided");
        return;
    }
    const auto effectFactoryIndex = random8(effects.size());
    const auto effectFactory = effects.at(effectFactoryIndex);

    const uint16_t layoutIndex = random16(displaySpec.nbLayouts());
    const auto effectMirror = catalog.randomMirror(layoutIndex);

    const auto [transitionLayoutIndex, transition] = catalog.randomTransition();
    const auto transitionMirror = catalog.randomMirror(transitionLayoutIndex);

    //TODO highlight

    const auto palette = PALETTES[random8(PALETTES.size())];

    auto effect = effectFactory(
        EffectContext(
            displaySpec.isCircular(),
            layoutIndex,
            Palette(palette, PaletteType::GRADIENT),
            effectMirror,
            transition,
            transitionLayoutIndex,
            transitionMirror
        )
    );

    if constexpr (IS_DEBUG) {
        Serial.println(
            "Layout: " + displaySpec.layoutName(layoutIndex)
            + "\t\tEffect: " + effect->name()
            + "\t\tMirror: " + getMirrorName(effectMirror)
        );
    }

    renderer->changeEffect(std::move(effect));
    // index = (index + 1) % displaySpec.nbLayouts();
}

void Display::render() const {
    renderer->render(outputArray);
    FastLED.show();
}

static uint32_t lastChangeTime = 0;

void Display::loop() {
    if (millis() - lastChangeTime >= currentEffectDurationsInSecs * 1000) {
        lastChangeTime = millis();
        currentEffectDurationsInSecs = random8(minEffectDurationsInSecs, maxEffectDurationsInSecs);

        if constexpr (IS_DEBUG) Serial.println("Next effect change in " + String(currentEffectDurationsInSecs) + "s");

        changeEffect();
    }

    EVERY_N_MILLISECONDS(refreshRateInMillis) {
        render();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy(freePinsForEntropy, nbPinsForEntropy);
    }
}
