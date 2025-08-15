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

#include "lib/engine/utils/Weights.h"

static const SlideTransitionFactory factoryInstance;
RenderableFactoryRef<uint8_t> SlideTransition::factory = &factoryInstance;

void SlideTransition::fillArrayInternal(
    uint8_t *renderableArray,
    uint16_t renderableArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    uint16_t limit = constrain(round((float) renderableArraySize * progress), 0, renderableArraySize);
    memset(renderableArray, CRGB::White, limit * sizeof(uint8_t));
    memset(renderableArray + limit, CRGB::Black, (renderableArraySize - limit) * sizeof(uint8_t));
}
