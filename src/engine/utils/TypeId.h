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

#ifndef TYPEID_H
#define TYPEID_H

namespace LEDSegments {

class TypeInfo {
public:
    // A lightweight, RTTI-free type identification system.
    using ID = uintptr_t;

    // Returns the next available unique ID.
    static ID next() {
        static ID next_id = 0;
        return next_id++;
    }
};

// Template to get a unique, static ID for any given type T.
template<typename T>
class TypeId {
public:
    static TypeInfo::ID id() {
        static TypeInfo::ID id_value = TypeInfo::next();
        return id_value;
    }
};

} // namespace LEDSegments

#endif // TYPEID_H
