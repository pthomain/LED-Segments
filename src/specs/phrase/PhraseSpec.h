//  SPDX-License-Identifier: GPL-3.0-or-later
//  Copyright (C) 2023 Pierre Thomain <pthomain@gmail.com>

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

#ifndef LED_SEGMENTS_PHRASESPEC_H
#define LED_SEGMENTS_PHRASESPEC_H

#define IS_TEST_PHRASE true

#if IS_TEST_PHRASE

#include "config/TestPhraseConfig.h"

#else
#include "specs/phrase/config/DlhPhraseConfig.h"
#endif

#include "engine/displayspec/DisplaySpec.h"
#include "specs/phrase/config/PhraseLayoutConfig.h"

class PhraseSpec : public DisplaySpec {
    void applyColourToLed(
        const uint16_t ledIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const;

public :
    explicit PhraseSpec(): DisplaySpec(phraseLayoutCatalog()) {
    }

    uint16_t nbLeds() const override { return NB_LEDS; }

    uint16_t nbLayouts() const override { return NB_LAYOUTS; }

    String layoutName(const uint16_t layoutIndex) const override;

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t nbPixels(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

    void setColour(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        const uint16_t frameIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const override;

    ~PhraseSpec() override = default;
};

#endif //LED_SEGMENTS_PHRASESPEC_H
