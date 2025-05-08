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

#include "SlideTransition.h"

EffectFactory<uint8_t> SlideTransition::factory = [](
    const EffectContext &effectContext
) -> std::unique_ptr<Effect> {
    return std::make_unique<SlideTransition>(effectContext);
};

void SlideTransition::fillArrayInternal(
    uint8_t *effectArray,
    uint16_t effectArraySize,
    float progress,
    unsigned long time
) {
    uint16_t limit = constrain(round((float) effectArraySize * progress), 0, effectArraySize);
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = i < limit ? 255 : 0;
    }
}
