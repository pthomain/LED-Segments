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

#pragma once
#ifndef EFFECTS_H
#define EFFECTS_H

#include "Renderable.h"

namespace LEDSegments {

template<typename Child, typename Data>
class TypedRenderable : public Renderable<Data> {
public :
    explicit TypedRenderable(const RenderableContext &context) : Renderable<Data>(context) {
        static_assert(
            std::is_base_of<TypedRenderable, Child>::value,
            "Child must be a subclass of TypedRenderable"
        );

        static_assert(
            std::is_same<decltype(Child::name()), const char *>::value,
            "Child class must implement static const char* name()"
        );

        static_assert(
            std::is_same<decltype(std::declval<Child>().operations()), WeightedOperations>::value,
            "Child class must implement WeightedOperations operations()"
        );
    };

    uint16_t param(uint8_t paramKey) const {
        auto parameters = this->context.parameters;
        if (parameters.find(paramKey) == parameters.end()) {
            #ifdef DEBUG
            Serial.printf("%s: Parameter %d not found\n", Child::name(), paramKey);
            #endif
            return 0;
        }

        return parameters.at(paramKey);
    }

    void beforeFrame(
        float progress,
        unsigned long timeElapsedInMillis
    ) override {
    }

    void afterFrame(
        float progress,
        unsigned long timeElapsedInMillis
    ) override {
    }

    WeightedOperations renderableOperations() override {
        return static_cast<Child *>(this)->operations();
    }

    static const char *name() {
        return Child::name();
    }
};

template<typename Child>
class Effect : public TypedRenderable<Child, CRGB> {
public:
    explicit Effect(const RenderableContext &context) :  TypedRenderable<Child, CRGB>(context) {
    }

   WeightedOperations operations() { return just(RenderableOperation::EFFECT); }
};

template<typename Child>
class Overlay : public TypedRenderable<Child, CRGB> {
public:
    explicit Overlay(const RenderableContext &context) : TypedRenderable<Child, CRGB>(context) {
    }
};

template<typename Child>
class Transition : public TypedRenderable<Child, uint8_t> {
public:
    explicit Transition(const RenderableContext &context) : TypedRenderable<Child, uint8_t>(context) {
    }
    WeightedOperations operations() { return just(RenderableOperation::TRANSITION); }
};

} // namespace LEDSegments

#endif //EFFECTS_H
