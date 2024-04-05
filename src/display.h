#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include "cluster.h"
#include "effects/effects.h"

#define IS_PROD true
#define LED_PIN 9

class Display {
private:
    Cluster letters;
    Cluster words;
    const int totalLeds;
    Cluster whole;
    const std::vector<Section> stripReversalSections;

    CRGB *allLeds;
    CRGB *bufferArray;

    Scope currentScope = SCOPE_WHOLE;

    void alignSections();

public:

    Display(
            Cluster letters,
            Cluster words,
            std::vector<Section> stripReversalSections,
            const int brightness
    );

    void changeEffect(
            const std::function<std::unique_ptr<Effect>(Section &, Mirror)> &effectFactory,
            const Scope scope,
            const PixelUnit pixelUnit,
            const Mirror mirror
    );

    void render();

    ~Display();
};

Display *initDisplay(const int brightness = -1);

#endif //DISPLAY_H