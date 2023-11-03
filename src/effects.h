#pragma once

#include <palettes.h>
#include <FastLED.h>
#include <vector>
#include <string>
#include <utility>
#include "utils.h"

class Effect {
private :
    const CRGBPalette16 palette;
//
//    // List of patterns to cycle through.  Each is defined as a separate function below.
//    typedef void (*SimplePatternList[2])();

    uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns

    void nextPattern() {
        // add one to the current pattern number, and wrap around at the end
        gCurrentPatternNumber = (gCurrentPatternNumber + 1) % 6;
    }

    void rainbow(CRGB *targetArray, int arraySize) {
        // FastLED's built-in rainbow generator
        fill_rainbow(targetArray, arraySize, gHue, 7);
    }

    void rainbowWithGlitter(CRGB *targetArray, int arraySize) {
        // built-in FastLED rainbow, plus some random sparkly glitter
        rainbow(targetArray, arraySize);
        addGlitter(targetArray, arraySize, 80);
    }

    void addGlitter(CRGB *targetArray, int arraySize, fract8 chanceOfGlitter) {
        if (random8() < chanceOfGlitter) {
            targetArray[random16(arraySize)] += CRGB::White;
        }
    }

    void confetti(CRGB *targetArray, int arraySize) {
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(targetArray, arraySize, 10);
        int pos = random16(arraySize);
        targetArray[pos] += CHSV(gHue + random8(64), 200, 255);
    }

    void sinelon(CRGB *targetArray, int arraySize) {
        // a colored dot sweeping back and forth, with fading trails
        fadeToBlackBy(targetArray, arraySize, 20);
        int pos = beatsin16(13, 0, arraySize - 1);
        targetArray[pos] += CHSV(gHue, 255, 192);
    }

    void bpm(CRGB *targetArray, int arraySize) {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < arraySize; i++) { //9948
            targetArray[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
        }
    }

    void juggle(CRGB *targetArray, int arraySize) {
        // eight colored dots, weaving in and out of sync with each other
        fadeToBlackBy(targetArray, arraySize, 20);
        uint8_t dothue = 0;
        for (int i = 0; i < 8; i++) {
            targetArray[beatsin16(i + 7, 0, arraySize - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }
//
//    SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};

public:
    Effect(
            const CRGBPalette16 &_palette
    ) : palette(_palette) {
    }

    void nextFrame(CRGB *targetArray, int arraySize) {
//        printNumber("nextFrame", arraySize);
//
//        for (int i = 0; i < arraySize; i++) {
//            targetArray[i] = ColorFromPalette(
//                    palette,
//                    map(i, 0, arraySize, 0, 255),
//                    255
//            );
//        }

//        gPatterns[gCurrentPatternNumber]();

        juggle(targetArray, arraySize);

        EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
//        EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically

        fadeToBlackBy(targetArray, arraySize, 20);
        uint8_t dothue = 0;
        for (int i = 0; i < 8; i++) {
            targetArray[beatsin16(i + 7, 0, arraySize - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }

};
