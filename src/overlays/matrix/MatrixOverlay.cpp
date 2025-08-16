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

#include "MatrixOverlay.h"
#include "crgb.h"
#include "engine/utils/Utils.h"
#include "engine/utils/Interpolation.h"

const uint8_t MatrixOverlay::PARAM_MIN_DENSITY;
const uint8_t MatrixOverlay::PARAM_MAX_DENSITY;
const uint8_t MatrixOverlay::PARAM_DENSITY_INCREMENT;
const uint8_t MatrixOverlay::PARAM_STREAM_LENGTH;
const uint8_t MatrixOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint8_t MatrixOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const MatrixOverlayFactory factoryInstance;
RenderableFactoryRef<CRGB> MatrixOverlay::factory = &factoryInstance;

MatrixOverlay::MatrixOverlay(const RenderableContext &context)
    : Overlay(context),
      densityIncrement(constrain(param(PARAM_DENSITY_INCREMENT), 0, 10)),
      minDensity(min(100, param(PARAM_MIN_DENSITY))),
      maxDensity(min(100, param(PARAM_MAX_DENSITY))),
      chanceOfNewStream(
          random8(
              min(minDensity, maxDensity) / densityIncrement,
              max(minDensity, maxDensity) / densityIncrement
          ) * (densityIncrement / 100.0f)
      ),
      streamLength(param(PARAM_STREAM_LENGTH)),
      multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
      invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
      streams(std::vector<std::vector<Stream> >(context.nbSegments)) {
}

void MatrixOverlay::fillArrayInternal(
    CRGB *renderableArray,
    uint16_t renderableArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    auto &segmentStreams = streams[segmentIndex];
    uint16_t speed = 2;

    if (frameIndex() % speed == 0) {
        // Handle streams
        segmentStreams.erase(
            std::remove_if(
                segmentStreams.begin(),
                segmentStreams.end(),
                [=](const Stream &stream) {
                    return !stream.isAlive;
                }
            ),
            segmentStreams.end()
        );

        // Add new stream if needed
        if (probability(chanceOfNewStream)) {
            segmentStreams.push_back({
                0,
                random8(2, streamLength),
                0.0f,
                (uint16_t) random8(2, 5) * 750,
                (uint16_t) timeElapsedInMillis,
                true
            });
        }
    }

    // Draw and update streams
    for (auto &stream: segmentStreams) {
        if (stream.isAlive) {
            stream.progress = (static_cast<float>(timeElapsedInMillis) - stream.startTime) / stream.duration;
            if (stream.progress > 1.0f) {
                stream.isAlive = false;
                continue;
            }

            float easedProgress = Interpolation::easeOutQuad(stream.progress);
            stream.position = easedProgress * (renderableArraySize + stream.length);

            for (int character = 0; character < stream.length; ++character) {
                int16_t charPos = stream.position - character;
                if (charPos >= 0 && charPos < renderableArraySize) {
                    float ageFade = 1.0f - stream.progress;
                    float brightness = ageFade * ageFade;

                    CRGB color = CRGB::White;
                    renderableArray[charPos] = color;//.nscale8(255 * brightness);
                }
            }
        }
    }
}
