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

#ifndef EYEOVERLAY_H
#define EYEOVERLAY_H

#include "lib/engine/render/renderable/TypedRenderable.h"
#include "lib/engine/render/renderable/RenderableOperation.h"
#include "lib/engine/render/renderable/BaseRenderableFactory.h"
#include "lib/engine/utils/Weights.h"

enum PupilPosition {
    C,
    T,
    B,
    TL,
    TR,
    BL,
    BR,
    L,
    R,
    CTL,
    CTR,
    CBL,
    CBR,
    CL,
    CR
};

class EyeOverlay : public TypedRenderable<EyeOverlay, CRGB> {

    const uint8_t eyeBrightness = 7;

    void drawPupil(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        unsigned long timeElapsedInMillis
    );

    uint8_t sequenceIndex = 0;
    uint8_t m_frameIndex = 0;
    unsigned long lastSequenceChange = 0;
    uint8_t previousSequenceIndex = 0;

    bool isBlinking = false;
    unsigned long lastBlinkTime = 0;
    uint8_t blinkFrameIndex = 0;

    unsigned long nextBlinkInterval = 5000;
    unsigned long nextSequenceInterval = 3000;
    unsigned long positionInSequence = 0;

public:
    explicit EyeOverlay(const RenderableContext &context): TypedRenderable(context) {
    }

    void fillArrayInternal(
        CRGB *renderableArray,
        uint16_t renderableArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "EyeOverlay"; }

    WeightedOperations operations() {
        return just(RenderableOperation::OVERLAY_MULTIPLY);
    }

    static RenderableFactoryRef<CRGB> factory;
};

class EyeOverlayFactory : public RenderableFactory<EyeOverlayFactory, EyeOverlay, CRGB> {
public:
    static Params declareParams() {
        return {};
    }
};

#endif //EYEOVERLAY_H