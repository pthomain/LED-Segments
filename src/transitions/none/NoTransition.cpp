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

#include "engine/utils/Weights.h"

static const NoTransitionFactory factoryInstance;
RenderableFactoryRef<uint8_t> NoTransition::factory = &factoryInstance;

void NoTransition::fillArrayInternal(
    uint8_t *renderableArray,
    uint16_t renderableArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    memset(renderableArray, progress < 0.5f ? 0 : 255, renderableArraySize * sizeof(uint8_t));
}