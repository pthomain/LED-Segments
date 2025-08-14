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

#ifndef COMPOSITEOVERLAY_H
#define COMPOSITEOVERLAY_H

#include "EyeOverlay.h"
#include "engine/displayspec/LayoutConfig.h"
#include "engine/effect/BaseEffectFactory.h"
#include "engine/effect/Effect.h"
#include "overlays/matrix/MatrixOverlay.h"
#include "overlays/sparkle/SparkleOverlay.h"

class CompositeOverlay : public Effect<CompositeOverlay, CRGB> {
    std::unique_ptr<BaseEffect> randomOverlay;
    EyeOverlay eyeOverlay;

public:
    explicit CompositeOverlay(
        const EffectContext &effectContext,
        EffectFactoryRef<CRGB> randomOverlayFactory
    ): Effect(effectContext),
       randomOverlay(randomOverlayFactory->create(effectContext)),
       eyeOverlay(EyeOverlay(effectContext)) {
        if constexpr (IS_DEBUG) {
            Serial.print("CompositeOverlay - random overlay: ");
            Serial.println(randomOverlayFactory->name());
        }
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "CompositeOverlay"; }

    WeightedOperations operations() {
        return just(EffectOperation::OVERLAY_MULTIPLY);
    }
};

class CompositeOverlayFactory : public EffectFactory<CompositeOverlayFactory, CompositeOverlay, CRGB> {
    const LayoutConfig *_config = nullptr;
    static EffectFactoryRef<CRGB> randomOverlayFactory;

public:
    std::unique_ptr<BaseEffect<CRGB> > createEffect(const EffectContext &context) const {
        randomOverlayFactory = std::get<1>(_config->randomOverlay());
        return std::make_unique<CompositeOverlay>(context, randomOverlayFactory);
    }

    void setConfig(const LayoutConfig *config) {
        _config = config;
        randomOverlayFactory = std::get<1>(_config->randomOverlay());
    }

    static Params declareParams() {
        return randomOverlayFactory->params();
    }
};

#endif //COMPOSITEOVERLAY_H
