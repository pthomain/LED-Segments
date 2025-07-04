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

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

Display::Display(
    CRGB *outputArray,
    std::unique_ptr<DisplaySpec> displaySpec,
    const std::vector<uint8_t> &freePinsForEntropy
) : outputArray(outputArray),
    _displaySpec(std::move(displaySpec)),
    renderer(std::make_unique<Renderer>(_displaySpec, outputArray)),
    freePinsForEntropy(freePinsForEntropy) {
    FastLED.setBrightness(_displaySpec->brightness);
    FastLED.clear(true);
    FastLED.show();
    addEntropy(freePinsForEntropy);
    changeEffect(random8(_displaySpec->minEffectDurationsInSecs, _displaySpec->maxEffectDurationsInSecs));
    render();
}

void Display::changeEffect(uint8_t effectDurationsInSecs) {
    const auto &catalog = _displaySpec->catalog;
    const Palette &palette = probability(_displaySpec->chanceOfRainbow)
                                 ? RAINBOW_PALETTE
                                 : PALETTES[random8(PALETTES.size())];

    const auto &[effectLayoutId, effectFactory, effectMirror] = catalog.randomEffect();
    const auto &[transitionLayoutId, transitionFactory, transitionMirror] = catalog.randomTransition();
    const auto &[overlayLayoutId, overlayFactory, overlayMirror] = catalog.randomOverlay();

    if constexpr (IS_DEBUG) {
        Serial.println("---");
        Serial.print("Palette\t\t\t");
        Serial.println(palette.name);
        Serial.println("-");
        Serial.print("Effect\t\t\t");
        Serial.println(effectFactory->name());
        Serial.print("Effect layout\t\t");
        Serial.println(catalog.layoutName(effectLayoutId));
        Serial.print("Effect mirror\t\t");
        Serial.println(getMirrorName(effectMirror));
        Serial.println("-");
        Serial.print("Overlay\t\t\t");
        Serial.println(overlayFactory->name());
        Serial.print("Overlay layout\t\t");
        Serial.println(catalog.layoutName(overlayLayoutId));
        Serial.print("Overlay mirror\t\t");
        Serial.println(getMirrorName(overlayMirror));
        Serial.println("-");
        Serial.print("Transition\t\t");
        Serial.println(transitionFactory->name());
        Serial.print("Transition layout\t");
        Serial.println(catalog.layoutName(transitionLayoutId));
        Serial.print("Transition mirror\t");
        Serial.println(getMirrorName(transitionMirror));
        Serial.println("---");
    }

    const uint16_t effectDurationInFrames = effectDurationsInSecs * _displaySpec->fps;
    auto transitionDurationInFrames = _displaySpec->fps * _displaySpec->transitionDurationInMillis / 1000;

    const auto effectContext = EffectContext(
        _displaySpec->maxSegmentSize(),
        _displaySpec->nbSegments(effectLayoutId),
        effectDurationInFrames,
        _displaySpec->isCircular,
        effectLayoutId,
        palette,
        effectMirror
    );

    const auto overlayContext = EffectContext(
        _displaySpec->maxSegmentSize(),
        _displaySpec->nbSegments(overlayLayoutId),
        effectDurationInFrames,
        _displaySpec->isCircular,
        overlayLayoutId,
        NO_PALETTE,
        overlayMirror
    );

    const auto transitionContext = EffectContext(
        _displaySpec->maxSegmentSize(),
        _displaySpec->nbSegments(transitionLayoutId),
        transitionDurationInFrames,
        _displaySpec->isCircular,
        transitionLayoutId,
        NO_PALETTE,
        transitionMirror
    );

    auto effect = effectFactory->create(effectContext);
    auto overlay = overlayFactory->create(overlayContext);
    auto transition = transitionFactory->create(transitionContext);

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
        changeEffect(random8(_displaySpec->minEffectDurationsInSecs, _displaySpec->maxEffectDurationsInSecs));
    }

    EVERY_N_MILLISECONDS(_displaySpec->refreshRateInMillis) {
        render();
    }

    EVERY_N_SECONDS(ENTROPY_UPDATE_IN_SECONDS) {
        addEntropy(freePinsForEntropy);
    }
}
