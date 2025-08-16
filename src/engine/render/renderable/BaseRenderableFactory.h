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

#ifndef RENDERABLEFACTORY_H
#define RENDERABLEFACTORY_H

#include "Renderable.h"
#include "engine/utils/TypeId.h"

using Params = std::map<uint8_t, uint16_t>;

template<typename C>
class BaseRenderableFactory {
public:
    const TypeInfo::ID id;

    virtual std::unique_ptr<Renderable<C> > create(const RenderableContext &context) const = 0;

    virtual const char *name() const = 0;

    template<typename T>
    bool is() const {
        return id == TypeId<T>::id();
    }

    bool is(TypeInfo::ID renderableId) const {
        return id == renderableId;
    }

    Params params(const std::function<uint16_t(uint8_t paramKey, uint16_t defaultValue)> &selector) const {
        auto parameters = Params();

        for (auto &[paramKey, defaultValue]: params()) {
            parameters.insert({paramKey, selector(paramKey, defaultValue)});
        }

        return parameters;
    }

    virtual Params params() const = 0;

    virtual ~BaseRenderableFactory() = default;

protected:
    explicit BaseRenderableFactory(TypeInfo::ID id) : id(id) {
    }
};

template<typename Child, typename Returned, typename Data>
class RenderableFactory : public BaseRenderableFactory<Data> {
protected:
    Params params() const override {
        return Child::declareParams();
    }

public:
    explicit RenderableFactory() : BaseRenderableFactory<Data>(TypeId<Returned>::id()) {
        static_assert(
            std::is_same<decltype(Child::declareParams()), Params>::value,
            "Child class must implement static Params declareParams()"
        );
    }

    std::unique_ptr<Renderable<Data> > create(const RenderableContext &context) const final {
        return static_cast<const Child *>(this)->createRenderable(context);
    }

    //Required for proper method dispatching with CRTP, override in child factory if needed
    std::unique_ptr<Renderable<Data> > createRenderable(const RenderableContext &context) const {
        return std::make_unique<Returned>(context);
    }

    const char *name() const override {
        return Returned::name();
    }
};

template<typename T>
using RenderableFactoryRef = const BaseRenderableFactory<T> *;

template<typename T>
using WeightedRenderable = WeightedItem<RenderableFactoryRef<T> >;

template<typename T>
using WeightedRenderables = std::vector<WeightedRenderable<T> >;

#endif //RENDERABLEFACTORY_H