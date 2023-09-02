#include <FastLED.h>
#include <constants.h>

int8_t letterSizes[] = {
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
    16,
};

CRGBSet letter0(leds(0, 15));
CRGBSet letter1(leds(16, 31));
CRGBSet letter2(leds(32, 47));
CRGBSet letter3(leds(48, 63));

CRGBSet letter4(leds(64, 79));
CRGBSet letter5(leds(80, 95));
CRGBSet letter6(leds(96, 111));
CRGBSet letter7(leds(112, 127));

CRGBSet letter8(leds(128, 143));
CRGBSet letter9(leds(144, 159));
CRGBSet letter10(leds(160, 175));
CRGBSet letter11(leds(176, 191));

CRGBSet letter12(leds(192, 207));
CRGBSet letter13(leds(208, 223));
CRGBSet letter14(leds(224, 239));
CRGBSet letter15(leds(240, 255));

struct CRGB *letters[] = {
    letter0,
    letter1,
    letter2,
    letter3,
    letter4,
    letter5,
    letter6,
    letter7,
    letter8,
    letter9,
    letter10,
    letter11,
    letter12,
    letter13,
    letter14,
    letter15};

void setLetterColour(int8_t index, CRGB colour)
{
    fill_solid(letters[index], letterSizes[index], colour);
}

void setLetterColour(int8_t index, CHSV colour)
{
    fill_solid(letters[index], letterSizes[index], colour);
}
