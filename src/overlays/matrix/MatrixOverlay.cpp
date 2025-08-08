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

const uint8_t MatrixOverlay::PARAM_MIN_DENSITY;
const uint8_t MatrixOverlay::PARAM_MAX_DENSITY;
const uint8_t MatrixOverlay::PARAM_DENSITY_INCREMENT;
const uint8_t MatrixOverlay::PARAM_STREAM_SPEED;
const uint8_t MatrixOverlay::PARAM_STREAM_LENGTH;
const uint8_t MatrixOverlay::PARAM_STREAM_LIFESPAN;
const uint8_t MatrixOverlay::PARAM_OPERATION_MULTIPLY_WEIGHT;
const uint8_t MatrixOverlay::PARAM_OPERATION_INVERT_WEIGHT;

static const MatrixOverlayFactory factoryInstance;
EffectFactoryRef<CRGB> MatrixOverlay::factory = &factoryInstance;

MatrixOverlay::MatrixOverlay(const EffectContext &effectContext)
    : Effect(effectContext),
      densityIncrement(constrain(param(PARAM_DENSITY_INCREMENT), 0, 10)),
      minDensity(min(100, param(PARAM_MIN_DENSITY))),
      maxDensity(min(100, param(PARAM_MAX_DENSITY))),
      chanceOfNewStream(
          random8(
              min(minDensity, maxDensity) / densityIncrement,
              max(minDensity, maxDensity) / densityIncrement
          ) * (densityIncrement / 100.0f)
      ),
      streamSpeed(param(PARAM_STREAM_SPEED)),
      streamLength(param(PARAM_STREAM_LENGTH)),
      streamLifespan(param(PARAM_STREAM_LIFESPAN)),
      multiplyOperationWeight(param(PARAM_OPERATION_MULTIPLY_WEIGHT)),
      invertOperationWeight(param(PARAM_OPERATION_INVERT_WEIGHT)),
      streams(std::vector<std::vector<Stream> >(context.nbSegments)) {
}

void MatrixOverlay::fillArrayInternal(
    CRGB *effectArray,
    uint16_t effectArraySize,
    uint16_t segmentIndex,
    float progress,
    unsigned long timeElapsedInMillis
) {
    auto &segmentStreams = streams[segmentIndex];

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
            random8(streamSpeed, streamSpeed * 2),
            random8(streamLength / 2, streamLength),
            random8(streamLifespan / 2, streamLifespan),
            0,
            true
        });
    }

    // Draw and update streams
    for (auto &stream: segmentStreams) {
        for (int character = 0; character < stream.length; ++character) {
            int16_t charPos = stream.position - character;
            if (charPos >= 0 && charPos < effectArraySize) {
                float tailFade = 1.0f - static_cast<float>(character) / stream.length;
                float ageFade = 1.0f - static_cast<float>(stream.age) / stream.lifespan;
                float brightness = tailFade * ageFade;

                CRGB color = CRGB::White;
                effectArray[charPos] = color.nscale8(255 * brightness);
            }
        }

        stream.position += stream.speed;
        if (stream.position - stream.length >= effectArraySize || stream.age >= stream.lifespan) {
            stream.isAlive = false;
        }
        stream.age++;
    }
}
