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

#ifndef NOEFFECT_H
#define NOEFFECT_H

#include "engine/effect/Effect.h"
#include "engine/utils/Weights.h"

class NoEffect : public Effect<NoEffect, CRGB> {
public:
    explicit NoEffect(const EffectContext &effectContext) : Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "NoEffect"; }
    static constexpr EffectOperation operation() { return EffectOperation::EFFECT; }
    static EffectFactoryRef<CRGB> factory;
};

class NoEffectFactory : public EffectFactory<CRGB> {
public:
    std::unique_ptr<BaseEffect<CRGB> > create(const EffectContext &context) const override {
        return std::make_unique<NoEffect>(context);
    }

    const char *name() const override {
        return NoEffect::name();
    }
};

#endif //NOEFFECT_H
