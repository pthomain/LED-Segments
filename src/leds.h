#pragma once

#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include "transformations.h"
#include "effects/effects.h"

#define IS_PROD true
#define LED_PIN 9

typedef enum {
    SCOPE_LETTER = 0,
    SCOPE_WORD = 1,
    SCOPE_WHOLE = 2
} Scope;

class Display {
private:
    Cluster *letters;
    Cluster *words;
    Cluster *whole;
    CRGB *allLeds;

public:

    Display(
            Cluster *letters,
            Cluster *words,
            int totalLeds,
            uint8_t brightness = 3
    );

    static void setBrightness(uint8_t brightness) {
        FastLED.setBrightness(brightness);
    }

    void render(Effect &effect, Scope scope);

    ~Display();
};

Display *initDisplay();
