#ifndef LED_SEGMENTS_PHRASESPEC_H
#define LED_SEGMENTS_PHRASESPEC_H

#include "displayspec/displayspec.h"
#include "FastLED.h"

static constexpr uint16_t NB_LEDS = 256;
static constexpr uint16_t LEDS_PER_ROW = 8;
static constexpr uint16_t NB_ROWS = NB_LEDS / LEDS_PER_ROW;
static constexpr uint16_t NB_LAYOUTS = 10;

static constexpr uint16_t NB_LETTERS = 10;
constexpr static const uint16_t LETTERS[10][2] = {
        {0,   23},
        {24,  39},
        {40,  63},
        {64,  103},
        {104, 151},
        {152, 207},
        {208, 231},
        {232, 239},
        {240, 247},
        {248, 255}
};

static constexpr uint16_t NB_WORDS = 3;
constexpr static const uint16_t WORDS[3][2] = {
        {0,   103},
        {104, 231},
        {232, 255}
};


// Format is PIXELS_IN_SEGMENTS
enum Layout {
    LEDS_IN_ROWS,
    LEDS_IN_LETTERS,
    LEDS_IN_WORDS,
    LEDS_IN_WHOLE,

    ROWS_IN_LETTERS,
    ROWS_IN_WORDS,
    ROWS_IN_WHOLE,

    LETTERS_IN_WORDS,
    LETTERS_IN_WHOLE,

    WORDS_IN_WHOLE
};

class PhraseSpec : public DisplaySpec {
private:

    bool hasSnakeRows = false;

public :

    explicit PhraseSpec(bool hasSnakeRows) : DisplaySpec(), hasSnakeRows(hasSnakeRows) {}

    uint16_t nbLeds() const override { return NB_LEDS; }

    uint16_t nbLayouts() const override { return NB_LAYOUTS; }

    String layoutName(const uint16_t layoutIndex) const override;

    uint16_t nbSegments(const uint16_t layoutIndex) const override;

    uint16_t segmentSize(const uint16_t layoutIndex, const uint16_t segmentIndex) const override;

    void setColour(
            const uint16_t start,
            const uint16_t end,
            const uint16_t pixelIndex,
            CRGB *outputArray,
            const CRGB colour
    ) const override;

    ~PhraseSpec() override = default;
};

#endif //LED_SEGMENTS_PHRASESPEC_H
