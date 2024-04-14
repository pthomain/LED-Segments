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
#include "effects/effect.h"

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
    CRGB *effectBufferArray;
    CRGB *modifierBufferArray;

    Scope currentScope = SCOPE_WHOLE;

    void alignSections();

public:

    Display(
            Cluster letters,
            Cluster words,
            std::vector<Section> stripReversalSections,
            const int brightness
    );

    void applyEffect(
            const std::function<Effect *(const Section &, const Mirror)> &effectFactory,
            const Scope scope,
            const PixelUnit pixelUnit,
            const Mirror mirror,
            const boolean isModifier
    );

    void render();

    ~Display();
};

Display *initDisplay(const int brightness = -1);

#endif //DISPLAY_H