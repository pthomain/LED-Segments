#pragma once

#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include "transformations.h"
#include "effects.h"

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
            const int totalLeds,
            const uint8_t brightness = 3
    ) :
            letters(letters),
            words(words),
            whole(new Cluster(new std::vector<Section>{Section(0, totalLeds - 1)})) {
        printNumber("LEDs", totalLeds);

        allLeds = new CRGB[totalLeds];

        Serial.println("Init strip");
        CFastLED::addLeds<WS2812B, LED_PIN, GRB>(allLeds, totalLeds);
        setBrightness(brightness);
    }

    static void setBrightness(uint8_t brightness) {
        FastLED.setBrightness(brightness);
    }

    void render(Effect effect, Scope scope) {
        printNumber("Effect", scope);

        switch (scope) {
            case SCOPE_LETTER:
                letters->applyTransformation(effect, allLeds);
                break;

            case SCOPE_WORD:
                words->applyTransformation(effect, allLeds);
                break;

            case SCOPE_WHOLE:
                whole->applyTransformation(effect, allLeds);
                break;
        }
        FastLED.show();
    }

    ~Display() {
        delete[] allLeds;
    }
};

Display *initDisplay() {
    if (IS_PROD) {
        return new Display(
                new Cluster(new std::vector<Section>(
                        {
                                std::make_pair(0, 12),
                                std::make_pair(13, 25),
                                std::make_pair(26, 41),
                                std::make_pair(42, 56),
                                std::make_pair(57, 74),
                                std::make_pair(75, 82),
                                std::make_pair(83, 93),
                                std::make_pair(94, 104),
                                std::make_pair(105, 117),
                                std::make_pair(118, 135),
                                std::make_pair(136, 143),
                                std::make_pair(144, 161),
                                std::make_pair(162, 177),
                                std::make_pair(178, 190)
                        })),
                new Cluster(new std::vector<Section>(
                        {
                                std::make_pair(0, 74),
                                std::make_pair(75, 82),
                                std::make_pair(83, 135),
                                std::make_pair(136, 143),
                                std::make_pair(144, 190)
                        })),
                191
        );
    } else {
        return new Display(
                new Cluster(new std::vector<Section>(
                        {
                                std::make_pair(0, 15),
                                std::make_pair(16, 31),
                                std::make_pair(32, 47),
                                std::make_pair(48, 63),
                                std::make_pair(64, 79),
                                std::make_pair(80, 95),
                                std::make_pair(96, 111),
                                std::make_pair(112, 127),
                                std::make_pair(128, 143),
                                std::make_pair(144, 159),
                                std::make_pair(160, 175),
                                std::make_pair(176, 191),
                                std::make_pair(192, 207),
                                std::make_pair(208, 223),
                                std::make_pair(224, 239),
                                std::make_pair(240, 255)
                        })),
                new Cluster(new std::vector<Section>(
                        {
                                std::make_pair(0, 63),
                                std::make_pair(64, 175),
                                std::make_pair(176, 255)
                        })),
                256
        );
    }
}
