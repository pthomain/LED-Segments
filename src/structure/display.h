#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#define FASTLED_USE_PROGMEM 1

#include "FastLED.h"
#include "utils/utils.h"
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include "cluster.h"
#include "effects/effect.h"
#include "render/fader.h"
#include "config/variation.h"

#define IS_PROD false
#define LED_PIN 9

class Display {
private:
    Cluster letters;
    Cluster words;
    const int totalLeds;
    Cluster whole;
    const std::vector<Section> stripReversalSections;

    CRGB *allLeds;
    Fader fader;

    void alignSections() const;

public:
    Display(
            Cluster letters,
            Cluster words,
            std::vector<Section> stripReversalSections,
            const int brightness
    );

    void pickNewEffect(
            const uint16_t transitionDurationInFrames,
            const std::vector<std::pair<std::function<Effect *(const Section &, Mirror, uint8_t)>, Variation>>& effectFactories
    );

    void render();

    ~Display() {
        delete[] allLeds;
    }
};

Display *initDisplay(const int brightness = -1);

#endif //DISPLAY_H
