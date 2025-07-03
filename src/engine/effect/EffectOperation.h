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

#ifndef EFFECTTYPE_H
#define EFFECTTYPE_H

enum class EffectType {
    EFFECT,
    OVERLAY,
    TRANSITION
};

enum class EffectOperation {
    EFFECT,
    OVERLAY_SOURCE,
    OVERLAY_SCREEN,
    OVERLAY_MULTIPLY,
    OVERLAY_INVERT,
    TRANSITION
};

inline String effectTypeName(EffectType effectType) {
    switch (effectType) {
        case EffectType::EFFECT:
            return "EFFECT";
        case EffectType::OVERLAY:
            return "OVERLAY";
        case EffectType::TRANSITION:
            return "TRANSITION";
        default:
            return "UNKNOWN";
    }
}

#endif //EFFECTTYPE_H
