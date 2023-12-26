#pragma once

#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include "cluster.h"
#include "effects/effects.h"

#define IS_PROD false
#define LED_PIN 9

typedef enum {
    UNIT_PIXEL,
    UNIT_COLUMN,
    UNIT_LETTER,
    UNIT_WORD
} PixelUnit;

class Display {
private:
    Cluster *columns;
    Cluster *letters;
    Cluster *words;
    std::vector<Section> *stripReversalSections;
    int totalLeds;
    Cluster *whole;

    void alignSections();

public:

    CRGB *allLeds;

    Display(
            Cluster *columns,
            Cluster *letters,
            Cluster *words,
            std::vector<Section> *stripReversalSections,
            int brightness
    );

    void render(Effect &effect, Scope scope, PixelUnit pixelUnit);

    ~Display();
};

Display *initDisplay();
