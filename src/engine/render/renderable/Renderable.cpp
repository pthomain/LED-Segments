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

#include "Renderable.h"

template<typename C>
void Renderable<C>::fillArray(
    C *renderableArray,
    uint16_t renderableArraySize,
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
        beforeFrame(progress, elapsedMillis);
    }

    if (renderableArraySize == 0) {
        Serial.println("Renderable::fillArray: empty array for segment " + String(segmentIndex));
        return;
    }

    memset(renderableArray, 0, renderableArraySize * sizeof(C));

    fillArrayInternal(
        renderableArray,
        renderableArraySize,
        segmentIndex,
        progress,
        elapsedMillis
    );

    if (segmentIndex == context.nbSegments - 1) {
        elapsedMillis = max(1, millis() - effectStartInMillis);
        afterFrame(progress, elapsedMillis);
        _frameIndex++;
    }
};
