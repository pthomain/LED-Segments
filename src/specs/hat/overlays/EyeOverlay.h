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

#include "engine/effect/BaseEffectFactory.h"
#include "engine/effect/Effect.h"
#include "engine/effect/EffectOperation.h"
#include "engine/utils/Weights.h"


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

class EyeOverlay : public Effect<EyeOverlay, CRGB> {
    static constexpr CRGB eyeDimming = CRGB(30, 30, 30);

    const std::vector<uint8_t> &pupilPosition(uint8_t position);

    void drawPupil(
        CRGB *effectArray,
        uint16_t effectArraySize,
        unsigned long timeElapsedInMillis
    );

    uint8_t m_sequenceIndex = 0;
    uint8_t m_frameIndex = 0;
    unsigned long m_lastSequenceChange = 0;
    uint8_t m_previousSequenceIndex = 0;

    bool m_isBlinking = false;
    unsigned long m_lastBlinkTime = 0;
    uint8_t m_blinkStateIndex = 0;

    unsigned long m_nextBlinkInterval = 5000;
    unsigned long m_nextSequenceInterval = 3000;
    unsigned long m_positionInSequence = 0;

public:
    explicit EyeOverlay(const EffectContext &effectContext): Effect(effectContext) {
    }

    void fillArrayInternal(
        CRGB *effectArray,
        uint16_t effectArraySize,
        uint16_t segmentIndex,
        float progress,
        unsigned long timeElapsedInMillis
    ) override;

    static constexpr const char *name() { return "EyeOverlay"; }

    WeightedOperations operations() {
        return just(EffectOperation::OVERLAY_MULTIPLY);
    }

    static EffectFactoryRef<CRGB> factory;
};

class EyeOverlayFactory : public EffectFactory<EyeOverlayFactory, EyeOverlay, CRGB> {
public:
    static Params declareParams() {
        return {};
    }
};

#endif //EYEOVERLAY_H
