#ifndef LED_SEGMENTS_PHRASESPEC_H
#define LED_SEGMENTS_PHRASESPEC_H

#include "config.h"

#if IS_TEST_PHRASE
#include "config/TestPhraseConfig.h"
#else
#include "config/DlhPhraseConfig.h"
#endif

#include "displayspec/displayspec.h"
#include "FastLED.h"

// Format is PIXELS_IN_SEGMENTS
enum Layout {
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,

    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,

    WORDS_IN_WHOLE
};
static constexpr uint16_t NB_LAYOUTS = 6;

class PhraseSpec : public DisplaySpec {
private:

    void applyColourToLed(
            const uint16_t ledIndex,
            CRGB *outputArray,
            const CRGB colour
    ) const;

public :

    explicit PhraseSpec() : DisplaySpec() {}

    uint16_t nbLeds() const override { return NB_LEDS; }

    uint16_t nbLayouts() const override { return NB_LAYOUTS; }

    String layoutName(const uint16_t layoutIndex) const override;

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

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
