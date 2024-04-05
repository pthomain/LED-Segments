#include "hueeffect.h"

const std::function<std::unique_ptr<Effect>(Section, Mirror)> HueEffect::factory = [](
        Section section,
        Mirror mirror
) -> std::unique_ptr<Effect> {
    return std::make_unique<HueEffect>(section, mirror);
};

void HueEffect::rainbowWithGlitter(CRGB *targetArray) {
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow(targetArray);
    addGlitter(targetArray, 80);
}

void HueEffect::addGlitter(CRGB *targetArray, fract8 chanceOfGlitter) {
    if (random8() < chanceOfGlitter) {
        targetArray[random16(arraySize)] += CRGB::White;
    }
}

void HueEffect::confetti(CRGB *targetArray) {
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(targetArray, arraySize, 10);
    int pos = random16(arraySize);
    targetArray[pos] += CHSV(hue + random8(64), 200, 255);
}

void HueEffect::sinelon(CRGB *targetArray) {
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(targetArray, arraySize, 20);
    int pos = beatsin16(140, 0, arraySize - 1);
    targetArray[pos] += CHSV(hue, 255, 192);
}

void HueEffect::bpm(CRGB *targetArray) {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    int BeatsPerMinute = 140;
    CRGBPalette16 palette = PartyColors_p;
    int beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < arraySize; i++) {
        //9948
        targetArray[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
    }
}

void HueEffect::juggle(CRGB *targetArray) {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(targetArray, arraySize, 20);
    int dothue = 0;
    for (int i = 0; i < 8; i++) {
        targetArray[beatsin16(i + 7, 0, arraySize - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void HueEffect::rainbow(CRGB *targetArray) {
    int step = max(1, 255 / arraySize);
    hue = (hue + 10) % 255;
    fill_rainbow(targetArray, arraySize, hue, step);
}

void HueEffect::rgbGradient(CRGB *targetArray) {
    fill_gradient_RGB(targetArray, arraySize, 0xFf0000, 0x00FF00);
}

void HueEffect::fillArray(CRGB *targetArray) {
    rainbow(targetArray);
}
