#include "hueeffect.h"


const std::function<std::unique_ptr<Effect>(Section)> HueEffect::factory = [](
        Section section) -> std::unique_ptr<Effect> {
    return std::make_unique<HueEffect>(section);
};

void HueEffect::nextPattern() {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % 6;
}

void HueEffect::rainbowWithGlitter(CRGB *targetArray, int start, int end) {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow(targetArray, start, end);
    addGlitter(targetArray, end - start + 1, 80);
}

void HueEffect::addGlitter(CRGB *targetArray, int arraySize, fract8 chanceOfGlitter) {
    if (random8() < chanceOfGlitter) {
        targetArray[random16(arraySize)] += CRGB::White;
    }
}

void HueEffect::confetti(CRGB *targetArray, int arraySize) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(targetArray, arraySize, 10);
    int pos = random16(arraySize);
    targetArray[pos] += CHSV(gHue + random8(64), 200, 255);
}

void HueEffect::sinelon(CRGB *targetArray, int arraySize) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(targetArray, arraySize, 20);
    int pos = beatsin16(13, 0, arraySize - 1);
    targetArray[pos] += CHSV(gHue, 255, 192);
}

void HueEffect::bpm(CRGB *targetArray, int arraySize) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    int BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    int beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < arraySize; i++) {
        //9948
        targetArray[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    }
}

void HueEffect::juggle(CRGB *targetArray, int arraySize) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(targetArray, arraySize, 20);
    int dothue = 0;
    for (int i = 0; i < 8; i++) {
        targetArray[beatsin16(i + 7, 0, arraySize - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void HueEffect::pong(
        CRGB *targetArray,
        int arraySize
) {
    for (int i = 0; i < arraySize; ++i) {
        if (i == position) {
            targetArray[i] = CRGB::White;
        } else {
            targetArray[i] = CRGB::Black;
        }
    }

    position = (position + 1) % arraySize;
}

void HueEffect::rainbow(CRGB *targetArray, int start, int end) {
    int arraySize = end - start + 1;
    int step = max(1, 255 / arraySize);

    //TODO fix this, the gradient should have a start and end

    fill_rainbow(targetArray, arraySize, gHue, step);
}

void HueEffect::rgbGradient(CRGB *targetArray, int start, int end) {
    int arraySize = end - start;
    fill_gradient_RGB(targetArray, arraySize, 0xFf0000, 0x00FF00);
}

void HueEffect::rgb(
        CRGB *targetArray,
        int start,
        int end
) {
    for (int x = 0; x <= end - start; x++) {
        CRGB colour;
        switch (x % 3) {
            case 0:
                colour = 0xFF0000;
                break;
            case 1:
                colour = 0x00FF00;
                break;
            default:
                colour = 0x0000FF;
        }
        targetArray[start + x] = colour;
    }
}

void HueEffect::rgbw(
        CRGB *targetArray,
        int start,
        int end
) {
    for (int x = 0; x <= end - start; x++) {
        CRGB colour;
        switch (x % 4) {
            case 0:
                colour = 0xFF0000;
                break;
            case 1:
                colour = 0x00FF00;
                break;
            case 2:
                colour = 0x0000FF;
                break;
            default:
                colour = 0xFFFFFF;
        }
        targetArray[start + x] = colour;
    }
}

void HueEffect::fillArray(CRGB *targetArray) {
    //        gPatterns[gCurrentPatternNumber]();
    rainbow(targetArray, section.start, section.end);

    //    EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
    //        EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
}
