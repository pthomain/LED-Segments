#include "PhraseSpec.h"
#include "functional"

uint16_t PhraseSpec::nbSegments(const uint16_t layoutIndex) const {
    switch (layoutIndex) {
        case LEDS_IN_ROWS:
            return NB_ROWS;

        case LEDS_IN_LETTERS:
        case ROWS_IN_LETTERS:
            return NB_LETTERS;

        case LEDS_IN_WORDS:
        case ROWS_IN_WORDS:
        case LETTERS_IN_WORDS:
            return NB_WORDS;

        default:   // LEDS_IN_WHOLE, ROWS_IN_WHOLE, LETTERS_IN_WHOLE, WORDS_IN_WHOLE
            return 1;
    }
}

uint16_t PhraseSpec::segmentSize(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex
) const {
    switch (layoutIndex) {
        case LEDS_IN_ROWS:
            return LEDS_PER_ROW;

        case LEDS_IN_LETTERS:
            return LETTERS[segmentIndex][1] - LETTERS[segmentIndex][0] + 1;

        case LEDS_IN_WORDS:
            return WORDS[segmentIndex][1] - WORDS[segmentIndex][0] + 1;

        case LEDS_IN_WHOLE:
            return NB_LEDS; //This makes the max segment size equal to NB_LEDS which isn't efficient

        case ROWS_IN_LETTERS:
            return segmentSize(LEDS_IN_LETTERS, segmentIndex) / LEDS_PER_ROW;

        case ROWS_IN_WORDS:
            return segmentSize(LEDS_IN_WORDS, segmentIndex) / LEDS_PER_ROW;

        case ROWS_IN_WHOLE:
            return NB_ROWS;

        case LETTERS_IN_WORDS:
            return segmentSize(LEDS_IN_WORDS, segmentIndex) / segmentSize(LEDS_IN_LETTERS, segmentIndex);

        case LETTERS_IN_WHOLE:
            return NB_LETTERS;

        case WORDS_IN_WHOLE:
            return NB_WORDS;

        default:
            return 0; //This should not happen
    }
}

void PhraseSpec::setColour(
        const uint16_t layoutIndex,
        const uint16_t segmentIndex,
        const uint16_t pixelIndex,
        CRGB *outputArray,
        CRGB colour
) const {
    auto applyColourToLed = [&](uint16_t ledIndex) {
        outputArray[ledIndex] = colour;
    };

    auto applyColourToRange = [&](uint16_t start, uint16_t end) {
        for (uint16_t i = start; i <= end; i++) {
            applyColourToLed(i);
        }
    };

    auto applyColourToPixel = [&](
            uint16_t segmentStart,
            uint16_t segmentEnd,
            uint16_t pixelArraySize,
            const std::function<std::array<uint16_t, 2>(uint16_t)> &getPixel
    ) {
        uint16_t containedPixelIndex = 0;
        for (uint8_t pixelIndex = 0; pixelIndex <= pixelArraySize; pixelIndex++) {
            auto pixel = getPixel(pixelIndex);
            if (pixel[0] >= segmentStart && pixel[1] <= segmentEnd) {
                if (containedPixelIndex == pixelIndex) {
                    applyColourToRange(pixel[0], pixel[1]);
                    return;
                }
                containedPixelIndex++;
            }
        }
    };

    auto rowSelector = [&](uint16_t pixelIndex) {
        uint16_t start = pixelIndex * LEDS_PER_ROW;
        uint16_t end = start + LEDS_PER_ROW - 1;
        return std::array<uint16_t, 2>{start, end};
    };

    switch (layoutIndex) {
        case LEDS_IN_ROWS:
            applyColourToLed(segmentIndex * LEDS_PER_ROW + pixelIndex);
            Serial.println("LEDS_IN_ROWS");
            break;

        case LEDS_IN_LETTERS:
            applyColourToLed(LETTERS[segmentIndex][0] + pixelIndex);
            Serial.println("LEDS_IN_LETTERS");
            break;

        case LEDS_IN_WORDS:
            applyColourToLed(WORDS[segmentIndex][0] + pixelIndex);
            Serial.println("LEDS_IN_WORDS");
            break;

        case LEDS_IN_WHOLE:
            applyColourToLed(pixelIndex);
            break;

        case ROWS_IN_LETTERS:
            applyColourToPixel(
                    LETTERS[segmentIndex][0],
                    LETTERS[segmentIndex][1],
                    NB_LETTERS,
                    rowSelector
            );
            break;

        case ROWS_IN_WORDS:
            applyColourToPixel(
                    WORDS[segmentIndex][0],
                    WORDS[segmentIndex][1],
                    NB_WORDS,
                    rowSelector
            );
            break;

        case ROWS_IN_WHOLE:
            applyColourToPixel(
                    0,
                    NB_LEDS - 1,
                    NB_ROWS,
                    rowSelector
            );
            break;

        case LETTERS_IN_WORDS:
            applyColourToPixel(
                    WORDS[segmentIndex][0],
                    WORDS[segmentIndex][1],
                    NB_LETTERS,
                    [&](uint16_t pixelIndex) {
                        return std::array<uint16_t, 2>{LETTERS[pixelIndex][0], LETTERS[pixelIndex][1]};
                    }
            );
            break;

        case LETTERS_IN_WHOLE:
            applyColourToRange(LETTERS[pixelIndex][0], LETTERS[pixelIndex][1]);
            break;

        case WORDS_IN_WHOLE:
            applyColourToRange(WORDS[pixelIndex][0], WORDS[pixelIndex][1]);
            break;
    };
}
