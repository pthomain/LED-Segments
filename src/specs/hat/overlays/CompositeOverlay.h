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
        const EffectContext &effectContext): Effect(effectContext),
       eyeOverlay(EyeOverlay(effectContext)) {
        // const auto &[overlayLayoutId, overlayFactory, overlayMirror] = config.randomOverlay();
        // randomOverlay = overlayFactory->create(context);
        randomOverlay = SparkleOverlay::factory->create(effectContext);
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

    static EffectFactoryRef<CRGB> factory;
};

class CompositeOverlayFactory : public EffectFactory<CompositeOverlayFactory, CompositeOverlay, CRGB> {

public:

    static Params declareParams() {
        return {};
    }
};

#endif //COMPOSITEOVERLAY_H
