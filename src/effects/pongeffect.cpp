#pragma once

#include "pongeffect.h"
#include "utils.h"

const std::function<std::unique_ptr<Effect>(Section)> PongEffect::factory = [](
        Section section) -> std::unique_ptr<Effect> {
    return std::make_unique<PongEffect>(section);
};

void PongEffect::fillArray(CRGB *targetArray) {
    const auto &str = "start: " + std::to_string(section.start)
                      + " end: " + std::to_string(section.end)
                      + " position: " + std::to_string(position);
//    Serial.println(str.c_str());

    for (int i = section.start; i <= section.end; ++i) {
        if (i == position) {
            targetArray[i] = CRGB::White;
        } else {
            targetArray[i] = CRGB::Black;
        }
    }

    isReversed ? --position : ++position;

    if (position == section.start || position == section.end) {
        isReversed = !isReversed;
    }

//    printNumber("position: ", position);
//    printNumber("length: ", section.sectionSize);
}
