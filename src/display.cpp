#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include "cluster.h"
#include "effects/effects.h"
#include "display.h"

Display::Display(
    Cluster* columns,
    Cluster* letters,
    Cluster* words,
    std::vector<Section>* stripReversalSections,
    const int brightness
) : columns(columns),
    letters(letters),
    words(words),
    stripReversalSections(stripReversalSections),
    totalLeds(words->sections->at(words->sections->size() - 1).end + 1),
    whole(new Cluster(new std::vector<Section>{Section(0, totalLeds - 1)}, SCOPE_WHOLE)) {
    allLeds = new CRGB[totalLeds];

    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(allLeds, totalLeds);
    FastLED.setBrightness(brightness);
    // FastLED.setCorrection(TypicalLEDStrip);
}

void Display::render(Effect& effect, Scope scope, PixelUnit pixelUnit) {
    switch (scope) {
        case SCOPE_COLUMN:
            columns->applyTransformation(effect, allLeds, totalLeds);
            break;

        case SCOPE_LETTER:
            if (pixelUnit == UNIT_COLUMN) {
                letters->applyTransformation(effect, allLeds, totalLeds, columns->sections);
            } else {
                letters->applyTransformation(effect, allLeds, totalLeds);
            }
            break;

        case SCOPE_WORD:
            if (pixelUnit == UNIT_COLUMN) {
                words->applyTransformation(effect, allLeds, totalLeds, columns->sections);
            } else if (pixelUnit == UNIT_LETTER) {
                words->applyTransformation(effect, allLeds, totalLeds, letters->sections);
            } else {
                words->applyTransformation(effect, allLeds, totalLeds);
            }
            break;

        case SCOPE_WHOLE:
            if (pixelUnit == UNIT_COLUMN) {
                words->applyTransformation(effect, allLeds, totalLeds, columns->sections);
            } else if (pixelUnit == UNIT_LETTER) {
                words->applyTransformation(effect, allLeds, totalLeds, letters->sections);
            } else if (pixelUnit == UNIT_WORD) {
                words->applyTransformation(effect, allLeds, totalLeds, words->sections);
            } else {
                whole->applyTransformation(effect, allLeds, totalLeds);
            }
            break;
    }

    // if (pixelUnit == UNIT_PIXEL) alignSections();

    FastLED.show();
}

void Display::alignSections() {
    if (stripReversalSections == nullptr)return;

    for (int i = 1; i < stripReversalSections->size(); i += 2) {
        auto section = stripReversalSections->at(i);
        int mid = (section.end - section.start) / 2;

        for (int j = 0; j <= mid; j++) {
            auto left = allLeds[section.start + j];
            auto right = allLeds[section.end - j];

            allLeds[section.start + j] = right;
            allLeds[section.end - j] = left;
        }
    }
}

Display::~Display() {
    delete[] allLeds;
}

Display* initDisplay() {
    if (IS_PROD) {
        return new Display(
            new Cluster(
                new std::vector<Section>(
                    {
                        Section(0, 12),
                        Section(13, 25),
                        Section(26, 41),
                        Section(42, 56),
                        Section(57, 74),
                        Section(75, 82),
                        Section(83, 93),
                        Section(94, 104),
                        Section(105, 117),
                        Section(118, 135),
                        Section(136, 143),
                        Section(144, 161),
                        Section(162, 177),
                        Section(178, 190)
                    }),
                SCOPE_COLUMN
            ),
            new Cluster(
                new std::vector<Section>(
                    {
                        Section(0, 12),
                        Section(13, 25),
                        Section(26, 41),
                        Section(42, 56),
                        Section(57, 74),
                        Section(75, 82),
                        Section(83, 93),
                        Section(94, 104),
                        Section(105, 117),
                        Section(118, 135),
                        Section(136, 143),
                        Section(144, 161),
                        Section(162, 177),
                        Section(178, 190)
                    }),
                SCOPE_LETTER
            ),
            new Cluster(
                new std::vector<Section>(
                    {
                        Section(0, 74),
                        Section(75, 82),
                        Section(83, 135),
                        Section(136, 143),
                        Section(144, 190)
                    }),
                SCOPE_WORD
            ),
            new std::vector<Section>({}),
            30
        );
    } else {
        auto columnSections = new std::vector<Section>(
            {
                Section(0, 7),
                Section(8, 15),
                Section(16, 23),
                Section(24, 31),
                Section(32, 39),
                Section(40, 47),
                Section(48, 55),
                Section(56, 63),
                Section(64, 71),
                Section(72, 79),
                Section(80, 87),
                Section(88, 95),
                Section(96, 103),
                Section(104, 111),
                Section(112, 119),
                Section(120, 127),
                Section(128, 135),
                Section(136, 143),
                Section(144, 151),
                Section(152, 159),
                Section(160, 167),
                Section(168, 175),
                Section(176, 183),
                Section(184, 191),
                Section(192, 199),
                Section(200, 207),
                Section(208, 215),
                Section(216, 223),
                Section(224, 231),
                Section(232, 239),
                Section(240, 247),
                Section(248, 255)
            });
        return new Display(
            new Cluster(columnSections, SCOPE_COLUMN),
            new Cluster(
                new std::vector<Section>(
                    {
                        Section(0, 15),
                        Section(16, 31),
                        Section(32, 47),
                        Section(48, 63),
                        Section(64, 79),
                        Section(80, 95),
                        Section(96, 111),
                        Section(112, 127),
                        Section(128, 143),
                        Section(144, 159),
                        Section(160, 175),
                        Section(176, 191),
                        Section(192, 207),
                        Section(208, 223),
                        Section(224, 239),
                        Section(240, 255)
                    }),
                SCOPE_LETTER
            ),
            new Cluster(
                new std::vector<Section>(
                    {
                        Section(0, 63),
                        Section(64, 175),
                        Section(176, 255)
                    }),
                SCOPE_WORD
            ),
            columnSections,
            1
        );
    }
}
