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

#include <FastLED.h>
#include "Display.h"

#include "engine/overlay/none/NoOverlay.h"
#include "engine/utils/Utils.h"
#include "engine/utils/seed/SeedGenerator.h"
#include "engine/transitions/fade/FadeTransition.h"
#include "engine/transitions/slide/SlideTransition.h"
#include "overlays/chase/ChaseOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
    CRGB *outputArray,
    std::unique_ptr<DisplaySpec> displaySpec,
    const uint8_t brightness,
    const uint8_t minEffectDurationsInSecs,
    const uint8_t maxEffectDurationsInSecs,
    const int16_t transitionDurationInMillis,
    const uint8_t fps,
    const std::vector<uint8_t> &freePinsForEntropy
) : outputArray(outputArray),
    minEffectDurationsInSecs(min(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
    maxEffectDurationsInSecs(max(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
    currentEffectDurationsInSecs(random8(minEffectDurationsInSecs, maxEffectDurationsInSecs)),
    fps(fps),
    transitionDurationInMillis(transitionDurationInMillis),
    refreshRateInMillis(fps == 0 ? 1000 : 1000 / fps),
    displaySpec(std::move(displaySpec)),
    renderer(std::make_unique<Renderer>(this->displaySpec, outputArray)),
    freePinsForEntropy(freePinsForEntropy) {
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    FastLED.show();
    addEntropy(freePinsForEntropy);
    changeEffect(random8(minEffectDurationsInSecs, maxEffectDurationsInSecs));
    render();
}

void Display::changeEffect(uint8_t effectDurationsInSecs) {
    const auto catalog = displaySpec->catalog();
    const Palette palette = probability(chanceOfRainbow) ? rainbowPalette : PALETTES[random8(PALETTES.size())];

    const uint16_t effectLayoutIndex = random16(catalog.nbLayouts());

    const auto effectFactory = catalog.randomEffectFactory(effectLayoutIndex);
    const auto effectMirror = catalog.randomMirror(effectLayoutIndex);

    auto [transitionLayoutIndex, transitionFactory] = catalog.randomTransition();
    const auto transitionMirror = catalog.randomMirror(transitionLayoutIndex);

    const auto [overlayLayoutIndex, overlayFactory] = catalog.randomOverlay();
    const uint16_t effectDurationInFrames = effectDurationsInSecs * fps;

    auto transitionDurationInFrames = fps * transitionDurationInMillis / 1000;

    EffectContext effectContext(
        displaySpec->maxSegmentSize(),
        displaySpec->nbSegments(effectLayoutIndex),
        effectDurationInFrames,
        displaySpec->isCircular(),
        effectLayoutIndex,
        palette,
        effectMirror
    );

    EffectContext overlayContext(
        displaySpec->maxSegmentSize(),
        displaySpec->nbSegments(overlayLayoutIndex),
        effectDurationInFrames,
        displaySpec->isCircular(),
        overlayLayoutIndex,
        NO_PALETTE,
        Mirror::NONE //TODO maybe add mirror
    );

    EffectContext transitionContext(
        displaySpec->maxSegmentSize(),
        displaySpec->nbSegments(transitionLayoutIndex),
        transitionDurationInFrames,
        displaySpec->isCircular(),
        transitionLayoutIndex,
        NO_PALETTE,
        transitionMirror
    );

    auto effect = effectFactory(effectContext);
    auto overlay = overlayFactory(overlayContext);
    auto transition = transitionFactory(transitionContext);

    if constexpr (IS_DEBUG) {
        Serial.print("Layout\t\t\t");
        Serial.println(catalog.layoutName(effectLayoutIndex));
        Serial.print("Effect\t\t\t");
        Serial.println(effect->name());
        Serial.print("Effect mirror\t\t");
        Serial.println(getMirrorName(effectMirror));
        Serial.print("Palette\t\t\t");
        Serial.println(palette.name);
        Serial.print("Overlay\t\t\t");
        Serial.println(overlay->name());
        Serial.print("Overlay layout\t\t");
        Serial.println(catalog.layoutName(overlayLayoutIndex));
        Serial.print("Transition\t\t");
        Serial.println(transition->name());
        Serial.print("Transition layout\t");
        Serial.println(catalog.layoutName(transitionLayoutIndex));
        Serial.print("Transition mirror\t");
        Serial.println(getMirrorName(transitionMirror));
        Serial.println("---");
    }

    renderer->changeEffect(std::move(effect), std::move(overlay), std::move(transition));

    currentEffectDurationsInSecs = effectDurationsInSecs;
    lastChangeTime = millis();
}

void Display::render() const {
    renderer->render();
    FastLED.show();
}

void Display::loop() {
    if (millis() - lastChangeTime >= currentEffectDurationsInSecs * 1000) {
        changeEffect(random8(minEffectDurationsInSecs, maxEffectDurationsInSecs));
    }

    EVERY_N_MILLISECONDS(refreshRateInMillis) {
        render();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy(freePinsForEntropy);
    }
}
