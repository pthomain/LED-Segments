#ifndef LED_SEGMENTS_PHRASESPEC_H
#define LED_SEGMENTS_PHRASESPEC_H

#define IS_TEST_PHRASE true

#if IS_TEST_PHRASE

#include "config/TestPhraseConfig.h"

#else
#include "specs/phrase/config/DlhPhraseConfig.h"
#endif

#include "engine/displayspec/displayspec.h"
#include "FastLED.h"

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,

    LETTERS_IN_WORDS,
    LEDS_IN_WORDS,

    WORDS_IN_WHOLE,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static constexpr uint16_t NB_LAYOUTS = 6;

class PhraseSpec : public DisplaySpec {
    // WORDS_IN_WHOLE: blocky transition, segments are too big
    // LEDS_IN_WHOLE: better to use LETTERS_IN_WHOLE in this case
    std::vector<uint16_t> transitionLayouts = std::vector<uint16_t>{
        0, //LEDS_IN_LETTERS
        1, //LETTERS_IN_WORDS
        2, //LEDS_IN_WORDS
        4 //LETTERS_IN_WHOLE
    };

    std::vector<uint16_t> allLayouts = std::vector<uint16_t>{
        0, //LEDS_IN_LETTERS,
        1, //LETTERS_IN_WORDS,
        // 2, //LEDS_IN_WORDS,
        3, //WORDS_IN_WHOLE,
        4, //LETTERS_IN_WHOLE,
        // 5 //LEDS_IN_WHOLE
    };

    void applyColourToLed(
        const uint16_t ledIndex,
        CRGB *outputArray,
        const CRGB colour
    ) const;

public :
    explicit PhraseSpec() = default;

    uint16_t nbLeds() const override { return NB_LEDS; }

    uint16_t nbLayouts() const override { return NB_LAYOUTS; }

    const std::vector<uint16_t> transitionLayoutIndexes() const override { return transitionLayouts; }

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

    std::vector<std::pair<EffectFactory, std::vector<uint16_t> > > getSupportedEffectFactories() const override;

    ~PhraseSpec() override = default;
};

#endif //LED_SEGMENTS_PHRASESPEC_H
