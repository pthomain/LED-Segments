#ifndef LED_SEGMENTS_PHRASESPEC_H
#define LED_SEGMENTS_PHRASESPEC_H

#define IS_TEST_PHRASE true

#if IS_TEST_PHRASE

#include "config/TestPhraseConfig.h"

#else
#include "specs/phrase/config/DlhPhraseConfig.h"
#endif

#include "engine/displayspec/displayspec.h"
#include "specs/phrase/config/PhraseLayoutConfig.h"

class PhraseSpec : public DisplaySpec {
    void applyColourToLed(
        const uint16_t ledIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const;

public :
    explicit PhraseSpec() = default;

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

    std::vector<EffectFactory> effects() const override { return supportedEffects; }
    std::vector<EffectFactory> highlights() const override { return NO_EFFECTS; }
    std::vector<uint16_t> matchLayouts(LayoutDescription description) const override;

    ~PhraseSpec() override = default;
};

#endif //LED_SEGMENTS_PHRASESPEC_H
