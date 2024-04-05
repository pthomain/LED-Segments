#pragma once

#include "pongeffect.h"
#include "utils.h"

const std::function<std::unique_ptr<Effect>(Section, Mirror)> PongEffect::factory = [](
        Section section,
        Mirror mirror
) -> std::unique_ptr<Effect> {
    return std::make_unique<PongEffect>(section, mirror);
};

void PongEffect::pong(CRGB *targetArray) {
//    printNumber("position", position);
    auto colour = isReversed ? CRGB::Green : CRGB::Red;

    for (int i = section.start; i <= section.end; ++i) {
        targetArray[i] = i == position ? colour : CRGB::Black;
    }

    isReversed ? --position : ++position;

    if (position == section.start || position == section.end) {
        isReversed = !isReversed;
    }
}

void PongEffect::movingDots(CRGB *targetArray) {
    uint16_t highest = section.sectionSize - 1;
    uint16_t posBeat = beatsin16(30, 0, highest, 0, 0);
    uint16_t posBeat2 = beatsin16(60, 0, highest, 0, 0);

    uint16_t posBeat3 = beatsin16(30, 0, highest, 0, 32767);
    uint16_t posBeat4 = beatsin16(60, 0, highest, 0, 32767);

    // Wave for LED color
    uint8_t colBeat = beatsin8(45, 0, 255, 0, 0);

    targetArray[(posBeat + posBeat2) / 2] = CHSV(colBeat, 255, 255);
    targetArray[(posBeat3 + posBeat4) / 2] = CHSV(colBeat, 255, 255);

    fadeToBlackBy(targetArray, section.sectionSize, 10);
}

void PongEffect::rainbowBeat(CRGB *targetArray) {
    uint16_t beatA = beatsin16(30, 0, 255);
    uint16_t beatB = beatsin16(20, 0, 255);
    fill_rainbow(targetArray, section.sectionSize, (beatA + beatB) / 2, 8);
}

void PongEffect::redWhiteBlue(CRGB *targetArray) {
    uint16_t highest = section.sectionSize - 1;
    uint16_t sinBeat = beatsin16(30, 0, highest, 0, 0);
    uint16_t sinBeat2 = beatsin16(30, 0, highest, 0, 21845);
    uint16_t sinBeat3 = beatsin16(30, 0, highest, 0, 43690);

    targetArray[sinBeat] = CRGB::Blue;
    targetArray[sinBeat2] = CRGB::Red;
    targetArray[sinBeat3] = CRGB::White;

    fadeToBlackBy(targetArray, section.sectionSize, 10);
}

void PongEffect::tricolourRainbow(CRGB *targetArray) {
    uint16_t highest = section.sectionSize - 1;
    uint16_t sinBeat = beatsin16(30, 0, highest, 0, 0);
    uint16_t sinBeat2 = beatsin16(30, 0, highest, 0, 21845);
    uint16_t sinBeat3 = beatsin16(30, 0, highest, 0, 43690);

    CHSV hsv;
    hsv.hue = hue;
    hsv.val = 255;
    hsv.sat = 240;

    targetArray[sinBeat] = hsv;
    hsv.hue += 85;
    targetArray[sinBeat2] = hsv;
    hsv.hue += 85;
    targetArray[sinBeat3] = hsv;

    hue += 30;
    fadeToBlackBy(targetArray, section.sectionSize, 10);
}

void PongEffect::party(CRGB *targetArray) {
    uint16_t beatA = beatsin16(19, 0, 255);
    uint16_t beatB = beatsin16(41, 0, 255);
    fill_palette(
            targetArray,
            section.sectionSize,
            (beatA + beatB) / 2,
            10,
            PartyColors_p,
            255,
            LINEARBLEND
    );
}

void PongEffect::fillArray(CRGB *targetArray) {
    party(targetArray);
}
