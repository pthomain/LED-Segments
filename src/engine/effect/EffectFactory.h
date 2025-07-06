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

#ifndef EFFECTFACTORY_H
#define EFFECTFACTORY_H

#include "BaseEffect.h"

template<typename C>
class EffectFactory {
public:
    virtual std::unique_ptr<BaseEffect<C> > create(const EffectContext &context) const = 0;

    virtual const char *name() const = 0;

    virtual ~EffectFactory() = default;
};

template<typename T>
using EffectFactoryRef = const EffectFactory<T> *;

template<typename T>
using WeightedEffect = WeightedItem<EffectFactoryRef<T> >;

template<typename T>
using WeightedEffects = std::vector<WeightedEffect<T> >;

#endif //EFFECTFACTORY_H
