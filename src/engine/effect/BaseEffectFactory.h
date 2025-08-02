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
#include "../utils/TypeId.h"

using Params = std::map<uint8_t, uint16_t>;

template<typename C>
class BaseEffectFactory {
public:
    const TypeInfo::ID effectId;

    virtual std::unique_ptr<BaseEffect<C> > create(const EffectContext &context) const = 0;

    virtual const char *name() const = 0;

    template<typename T>
    bool is() const {
        return effectId == TypeId<T>::id();
    }

    bool is(TypeInfo::ID effectId) const {
        return this->effectId == effectId;
    }

    Params params(const std::function<uint16_t(uint8_t paramKey, uint16_t defaultValue)> &selector) const {
        auto parameters = Params();

        for (auto &[paramKey, defaultValue]: params()) {
            parameters.insert({paramKey, selector(paramKey, defaultValue)});
        }

        return parameters;
    }

    virtual Params params() const = 0;

    virtual ~BaseEffectFactory() = default;

protected:
    explicit BaseEffectFactory(TypeInfo::ID id) : effectId(id) {
    }
};

template<typename Child, typename T, typename C>
class EffectFactory : public BaseEffectFactory<C> {
protected:
    Params params() const override {
        return Child::declareParams();
    }

public:
    explicit EffectFactory() : BaseEffectFactory<C>(TypeId<T>::id()) {
        static_assert(
            std::is_same<decltype(Child::declareParams()), Params>::value,
            "Child class must implement static Params declareParams()"
        );
    }

    std::unique_ptr<BaseEffect<C> > create(const EffectContext &context) const override {
        return std::make_unique<T>(context);
    }

    const char *name() const override {
        return T::name();
    }
};

template<typename T>
using EffectFactoryRef = const BaseEffectFactory<T> *;

template<typename T>
using WeightedEffect = WeightedItem<EffectFactoryRef<T> >;

template<typename T>
using WeightedEffects = std::vector<WeightedEffect<T> >;

#endif //EFFECTFACTORY_H
