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

#include "NoTransition.h"

EffectFactory<uint8_t> NoTransition::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<NoTransition>(effectContext);
};

void NoTransition::fillArrayInternal(
    uint8_t *effectArray,
    uint16_t effectArraySize,
    float progress,
    unsigned long time
) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = progress < 0.5f ? 0 : 255;
    }
}
