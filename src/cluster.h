#pragma once

#include <utility>
#include <vector>
#include <FastLED.h>
#include <effects/effects.h>

class Section {
public:
    int start;
    int end;
    int sectionSize;

    Section(int start, int end)
        : start(start), end(end), sectionSize(end - start) {
    }
};

class Pixel {
public:
    Section section;
    CRGB colour;

    Pixel(Section section, CRGB colour)
        : section(section), colour(colour) {
    }
};

typedef enum {
    LTR = 0,
    RTL = 1,
} Direction;

typedef enum {
    MIRROR_CENTRE = 0,
    MIRROR_EDGE = 1,
    MIRROR_NONE = 2,
} Mirror;

typedef enum {
    SCOPE_COLUMN = 0,
    SCOPE_LETTER = 1,
    SCOPE_WORD = 2,
    SCOPE_WHOLE = 3
} Scope;

class Cluster {
private:
    Scope scope;

public:
    std::vector<Section>* sections;
    std::vector<std::pair<CRGB *, Section>> sectionArrays;

    Cluster(std::vector<Section>* sections, Scope scope);

    void applyTransformation(
        Effect& effect,
        CRGB* targetArray,
        int targetArraySize,
        std::vector<Section>* pixelSections = nullptr
    );
};
