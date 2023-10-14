#pragma once

#include <FastLED.h>
#include <string> // for std::string

#define LED_PIN 9
#define LED_TYPE WS2812B
#define LED_BRIGHTNESS 6
#define COLOR_ORDER GRB

struct Cluster {
    Cluster(uint8_t start, uint8_t end) {
        this->start = start;
        this->end = end;
    }

    uint8_t start;
    uint8_t end;
};

const Cluster letters[] = {
        Cluster(0, 15),
        Cluster(16, 31),
        Cluster(32, 47),
        Cluster(48, 63),
        Cluster(64, 79),
        Cluster(80, 95),
        Cluster(96, 111),
        Cluster(112, 127),
        Cluster(128, 143),
        Cluster(144, 159),
        Cluster(160, 175),
        Cluster(176, 191),
        Cluster(192, 207),
        Cluster(208, 223),
        Cluster(224, 239),
        Cluster(240, 255)
};

const Cluster words[]{
        Cluster(letters[0].start, letters[3].end),
        Cluster(letters[4].start, letters[10].end),
        Cluster(letters[11].start, letters[15].end),
};

const uint8_t NUM_LETTERS = sizeof(letters) / sizeof(Cluster);
const uint8_t NUM_WORDS = sizeof(words) / sizeof(Cluster);

const Cluster whole = Cluster(0, letters[NUM_LETTERS - 1].end);
static const uint8_t NUM_LEDS = whole.end;

CRGB *leds;

void printNumber(char text[], uint8_t value) {
    auto string = std::string(text);
    string.append(": ");
    string.append(std::to_string(value));
    Serial.println(string.data());
}

void initLeds() {
    Serial.println("Setup");
    printNumber("Letters", NUM_LETTERS);
    printNumber("Words", NUM_WORDS);
    printNumber("LEDs", NUM_LEDS);

    Serial.println("Init array");
    CRGB rawleds[NUM_LEDS];
    leds = CRGBSet(rawleds, NUM_LEDS);

    Serial.println("Init strip");
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();

    Serial.println("Init done");
}
