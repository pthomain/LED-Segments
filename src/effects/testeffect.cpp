#pragma once

#include "testeffect.h"
#include "utils.h"

//const std::function<std::unique_ptr<Effect>(Modifier *)> TestEffect::factory = [](
//        Modifier &modifier
//) -> std::unique_ptr<Effect> {
//    return std::make_unique<TestEffect>(modifier);
//};
//
//void TestEffect::confetti(CRGB *targetArray) {
//    // random colored speckles that blink in and fade smoothly
//    fadeToBlackBy(targetArray, arraySize, 10);
//    int pos = random16(arraySize);
//    targetArray[pos] += CHSV(hue + random8(64), 200, 255);
//}
//
//void TestEffect::sinelon(CRGB *targetArray) {
//    // a colored dot sweeping back and forth, with fading trails
//    fadeToBlackBy(targetArray, arraySize, 20);
//    int pos = beatsin16(140, 0, arraySize - 1);
//    targetArray[pos] += CHSV(hue, 255, 192);
//}
//
//void TestEffect::bpm(CRGB *targetArray) {
//    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
//    int BeatsPerMinute = 140;
//    CRGBPalette16 palette = PartyColors_p;
//    int beat = beatsin8(BeatsPerMinute, 64, 255);
//    for (int i = 0; i < arraySize; i++) {
//        //9948
//        targetArray[i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
//    }
//}
//
//void TestEffect::juggle(CRGB *targetArray) {
//    // eight colored dots, weaving in and out of sync with each other
//    fadeToBlackBy(targetArray, arraySize, 20);
//    int dothue = 0;
//    for (int i = 0; i < 8; i++) {
//        targetArray[beatsin16(i + 7, 0, arraySize - 1)] |= CHSV(dothue, 200, 255);
//        dothue += 32;
//    }
//}
//
//void TestEffect::rainbow(CRGB *targetArray) {
//    int step = max(1, 255 / arraySize);
//    hue = (hue + 10) % 255;
//    fill_rainbow(targetArray, arraySize, hue, step);
//}
//
//void TestEffect::movingDots(CRGB *targetArray) {
//    uint16_t highest = arraySize - 1;
//    uint16_t posBeat = beatsin16(30, 0, highest, 0, 0);
//    uint16_t posBeat2 = beatsin16(60, 0, highest, 0, 0);
//
//    uint16_t posBeat3 = beatsin16(30, 0, highest, 0, 32767);
//    uint16_t posBeat4 = beatsin16(60, 0, highest, 0, 32767);
//
//    // Wave for LED color
//    uint8_t colBeat = beatsin8(45, 0, 255, 0, 0);
//
//    targetArray[(posBeat + posBeat2) / 2] = CHSV(colBeat, 255, 255);
//    targetArray[(posBeat3 + posBeat4) / 2] = CHSV(colBeat, 255, 255);
//
//    fadeToBlackBy(targetArray, arraySize, 10);
//}
//
//void TestEffect::rainbowBeat(CRGB *targetArray) {
//    uint16_t beatA = beatsin16(30, 0, 255);
//    uint16_t beatB = beatsin16(20, 0, 255);
//    fill_rainbow(targetArray, arraySize, (beatA + beatB) / 2, 8);
//}
//
//void TestEffect::redWhiteBlue(CRGB *targetArray) {
//    uint16_t highest = arraySize - 1;
//    uint16_t sinBeat = beatsin16(30, 0, highest, 0, 0);
//    uint16_t sinBeat2 = beatsin16(30, 0, highest, 0, 21845);
//    uint16_t sinBeat3 = beatsin16(30, 0, highest, 0, 43690);
//
//    targetArray[sinBeat] = CRGB::Blue;
//    targetArray[sinBeat2] = CRGB::Red;
//    targetArray[sinBeat3] = CRGB::White;
//
//    fadeToBlackBy(targetArray, arraySize, 10);
//}
//
//void TestEffect::tricolourRainbow(CRGB *targetArray) {
//    uint16_t highest = arraySize - 1;
//    uint16_t sinBeat = beatsin16(30, 0, highest, 0, 0);
//    uint16_t sinBeat2 = beatsin16(30, 0, highest, 0, 21845);
//    uint16_t sinBeat3 = beatsin16(30, 0, highest, 0, 43690);
//
//    CHSV hsv;
//    hsv.hue = hue;
//    hsv.val = 255;
//    hsv.sat = 240;
//
//    targetArray[sinBeat] = hsv;
//    hsv.hue += 85;
//    targetArray[sinBeat2] = hsv;
//    hsv.hue += 85;
//    targetArray[sinBeat3] = hsv;
//
//    hue += 30;
//    fadeToBlackBy(targetArray, arraySize, 10);
//}
//
//void TestEffect::fillArray(CRGB *targetArray) {
//}
