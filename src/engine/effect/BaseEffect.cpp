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

#include "Effect.h"

template<typename C>
void BaseEffect<C>::fillArray(
    C *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress
) {
    unsigned long elapsedMillis;
    if (_frameIndex == 0) {
        effectStartInMillis = millis();
        elapsedMillis = 0;
    } else {
        elapsedMillis = max(1, millis() - effectStartInMillis);
    }

    if (segmentIndex == 0) {
        onEachFrame(progress, elapsedMillis);
        _frameIndex++;
    }

    if (effectArraySize == 0) {
        Serial.println("BaseEffect::fillArray: empty array for segment " + String(segmentIndex));
        return;
    }

    memset(effectArray, 0, effectArraySize * sizeof(C));

    fillArrayInternal(
        effectArray,
        effectArraySize,
        segmentIndex,
        progress,
        elapsedMillis
    );
};
