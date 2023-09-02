#include <FastLED.h>
#include <letters.h>

uint16_t x = 0;
uint16_t time = 0;
uint8_t scale = 40;
uint8_t octave = 1;
uint8_t noiseData[NUM_LETTERS];

int8_t deltaHue = 255 / NUM_LETTERS;
int8_t currentHue = 0;

CRGBPalette16 currentPalette(OceanColors_p);
CRGBPalette16 targetPalette(LavaColors_p);

void fillRainbow(uint8_t initialhue)
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    for (int i = 0; i < 16; ++i)
    {
        setLetterColour(i, hsv);
        hsv.hue += deltaHue;
    }
}

int8_t normaliseNoise(int8_t value)
{
    return map(value, 30, 195, 0, 255);
}

void showRainbowSwipe(bool mirror = false)
{
    EVERY_N_MILLISECONDS(100)
    {
        fillRainbow(currentHue);
        currentHue = beat8(60);
        FastLED.show();
    }
}

void showNoisePalette(bool mirror = false)
{
    EVERY_N_SECONDS(5)
    { // Change the target palette to a random one every 5 seconds.
        targetPalette = CRGBPalette16(
            CHSV(random8(), 255, random8(128, 255)),
            CHSV(random8(), 255, random8(128, 255)),
            CHSV(random8(), 192, random8(128, 255)),
            CHSV(random8(), 255, random8(128, 255)));
    }

    EVERY_N_MILLISECONDS(100)
    {
        time = millis() / 10;
        x = beatsin8(11, 0, NUM_LETTERS);

        memset(noiseData, 0, NUM_LETTERS);
        fill_raw_noise8(noiseData, NUM_LETTERS, octave, x, scale, time);

        for (int8_t i = 0; i < NUM_LETTERS; i++)
        {
            auto colour = ColorFromPalette(targetPalette, normaliseNoise(noiseData[i]), 255);
            setLetterColour(i, colour);
        }

        FastLED.show();
    }
}