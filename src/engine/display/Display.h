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
#include "engine/utils/Utils.h"
#include "engine/displayspec/DisplaySpec.h"
#include "engine/render/Renderer.h"
#include <algorithm> // Required for std::remove and std::erase
#include <type_traits>
#include "overlays/none/NoOverlay.h"
#include "engine/utils/seed/SeedGenerator.h"

#define FASTLED_USE_PROGMEM 1
#define ENTROPY_UPDATE_IN_SECONDS 5

template<typename, typename = void>
struct has_led_pin : std::false_type {
};

template<typename, typename = void>
struct has_rgb_order : std::false_type {
};

template<typename T>
struct has_led_pin<T, std::void_t<decltype(T::LED_PIN)> > : std::true_type {
};

template<typename T>
struct has_rgb_order<T, std::void_t<decltype(T::RGB_ORDER)> > : std::true_type {
};

template<typename SPEC>
class Display {
    static_assert(std::is_base_of_v<DisplaySpec, SPEC>, "Template parameter SPEC must be a subclass of DisplaySpec");
    static_assert(has_led_pin<SPEC>::value, "SPEC must provide a static constexpr int LED_PIN");
    static_assert(has_rgb_order<SPEC>::value, "SPEC must provide a static constexpr EOrder RGB_ORDER");
    std::shared_ptr<SPEC> _displaySpec;
    CRGB *outputArray;
    const std::unique_ptr<Renderer> renderer;

public:
    uint32_t lastChangeTime = 0;
    uint8_t currentEffectDurationsInSecs = 0;

    explicit Display(
        //change if any of those pins are already in use or unavailable on the board
        std::vector<uint8_t> freePinsForEntropy = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    ) : _displaySpec(std::unique_ptr<SPEC>(new SPEC())),
        outputArray(new CRGB[_displaySpec->nbLeds()]),
        renderer(std::make_unique<Renderer>(_displaySpec, outputArray)) {
        freePinsForEntropy.erase(
            std::remove(freePinsForEntropy.begin(), freePinsForEntropy.end(), SPEC::LED_PIN),
            freePinsForEntropy.end()
        );
        addEntropy(freePinsForEntropy);

        CFastLED::addLeds<WS2812B, SPEC::LED_PIN, SPEC::RGB_ORDER>(outputArray, _displaySpec->nbLeds())
                .setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(_displaySpec->brightness);
        FastLED.clear(true);
        FastLED.show();

        changeEffect(random8(_displaySpec->minEffectDurationsInSecs, _displaySpec->maxEffectDurationsInSecs));
        render();
    }

    void changeEffect(uint8_t effectDurationsInSecs) {
        const auto &config = _displaySpec->config;
        const Palette &palette = probability(_displaySpec->chanceOfRainbow)
                                     ? RAINBOW_PALETTE
                                     : PALETTES[random8(PALETTES.size())];

        const auto &[effectLayoutId, effectFactory, effectMirror] = config.randomEffect();
        const auto &[transitionLayoutId, transitionFactory, transitionMirror] = config.randomTransition();
        const auto &[overlayLayoutId, overlayFactory, overlayMirror] = config.randomOverlay();

        if constexpr (IS_DEBUG) {
            Serial.println("---");
            Serial.print("Palette			");
            Serial.println(palette.name);
            Serial.println("-");
            Serial.print("Effect			");
            Serial.println(effectFactory->name());
            Serial.print("Effect layout		");
            Serial.println(config.layoutName(effectLayoutId));
            Serial.print("Effect mirror		");
            Serial.println(getMirrorName(effectMirror));
            Serial.println("-");
            Serial.print("Overlay			");
            Serial.println(overlayFactory->name());
            Serial.print("Overlay layout		");
            Serial.println(config.layoutName(overlayLayoutId));
            Serial.print("Overlay mirror		");
            Serial.println(getMirrorName(overlayMirror));
            Serial.println("-");
            Serial.print("Transition		");
            Serial.println(transitionFactory->name());
            Serial.print("Transition layout	");
            Serial.println(config.layoutName(transitionLayoutId));
            Serial.print("Transition mirror	");
            Serial.println(getMirrorName(transitionMirror));
            Serial.println("---");
        }

        const uint16_t effectDurationInFrames = effectDurationsInSecs * _displaySpec->fps;
        auto transitionDurationInFrames = _displaySpec->fps * _displaySpec->transitionDurationInMillis / 1000;

        auto effectParams = config.params(
            EffectType::EFFECT,
            effectFactory->effectId,
            effectMirror
        );

        if (effectParams.empty()) effectParams = effectFactory->params();

        const auto effectContext = EffectContext(
            _displaySpec->maxSegmentSize(),
            _displaySpec->nbSegments(effectLayoutId),
            effectDurationInFrames,
            _displaySpec->isCircular,
            effectLayoutId,
            palette,
            effectMirror,
            effectParams
        );

        auto overlayParams = config.params(
            EffectType::OVERLAY,
            overlayFactory->effectId,
            overlayMirror
        );

        if (overlayParams.empty()) overlayParams = overlayFactory->params();

        const auto overlayContext = EffectContext(
            _displaySpec->maxSegmentSize(),
            _displaySpec->nbSegments(overlayLayoutId),
            effectDurationInFrames,
            _displaySpec->isCircular,
            overlayLayoutId,
            NO_PALETTE,
            overlayMirror,
            overlayParams
        );

        auto transitionParams = config.params(
            EffectType::TRANSITION,
            transitionFactory->effectId,
            transitionMirror
        );

        if (transitionParams.empty()) transitionParams = transitionFactory->params();

        const auto transitionContext = EffectContext(
            _displaySpec->maxSegmentSize(),
            _displaySpec->nbSegments(transitionLayoutId),
            transitionDurationInFrames,
            _displaySpec->isCircular,
            transitionLayoutId,
            NO_PALETTE,
            transitionMirror,
            transitionParams
        );

        auto effect = effectFactory->create(effectContext);
        auto overlay = overlayFactory->create(overlayContext);
        auto transition = transitionFactory->create(transitionContext);

        renderer->changeEffect(std::move(effect), std::move(overlay), std::move(transition));

        currentEffectDurationsInSecs = effectDurationsInSecs;
        lastChangeTime = millis();
    }

    void render() const {
        renderer->render();
        FastLED.show();
    }

    void loop() {
        if (millis() - lastChangeTime >= currentEffectDurationsInSecs * 1000) {
            changeEffect(random8(_displaySpec->minEffectDurationsInSecs, _displaySpec->maxEffectDurationsInSecs));
        }

        EVERY_N_MILLISECONDS(_displaySpec->refreshRateInMillis) {
            render();
        }
    }

    ~Display() {
        delete[] outputArray;
    };
};

#endif //LED_SEGMENTS_DISPLAY_H
