#include <FastLED.h>
#include <utils.h>
#include <utility>
#include <vector>
#include "cluster.h"
#include "effects/effect.h"
#include "display.h"
#include "config/effectconfig.h"

Display::Display(
        Cluster letters,
        Cluster words,
        std::vector<Section> stripReversalSections,
        const int brightness
) : letters(std::move(letters)),
    words(std::move(words)),
    totalLeds(this->words.clusterSection.end + 1),
    whole(Cluster(
                  std::vector<Section>(
                          std::vector<Section>{Section(0, totalLeds - 1)}
                  ),
                  SCOPE_WHOLE
          )
    ),
    stripReversalSections(std::move(stripReversalSections)) {

    allLeds = new CRGB[totalLeds];
    effectBufferArray = new CRGB[totalLeds];
    modifierBufferArray = new CRGB[totalLeds];

    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(allLeds, totalLeds);
    FastLED.setBrightness(brightness);
}

void Display::applyEffectOrModifier(
        const std::function<Effect *(const Section &, const Mirror)> &effectFactory,
        const Scope scope,
        const PixelUnit pixelUnit,
        const Mirror mirror,
        const boolean isModifier
) {
    currentScope = scope;
    std::string output = (isModifier ? "MODIFIER\t" : "EFFECT\t\t") + scopeToString(scope)
                         + "\t" + pixelUnitToString(pixelUnit)
                         + "\t" + mirrorToString(mirror);
    Serial.println(output.c_str());

    switch (scope) {
        case SCOPE_LETTER: {
            auto *config = new EffectConfig(effectFactory, nullptr, mirror, isModifier);
            letters.applyConfig(config);
            delete config;
        }
            break;

        case SCOPE_WORD:
            if (pixelUnit == UNIT_LETTER) {
                auto *config = new EffectConfig(effectFactory, &letters, mirror, isModifier);
                words.applyConfig(config);
                delete config;
            } else {
                auto *config = new EffectConfig(effectFactory, nullptr, mirror, isModifier);
                words.applyConfig(config);
                delete config;
            }
            break;

        case SCOPE_WHOLE:
            if (pixelUnit == UNIT_WORD) {
                auto *config = new EffectConfig(effectFactory, &words, mirror, isModifier);
                whole.applyConfig(config);
                delete config;
            } else if (pixelUnit == UNIT_LETTER) {
                auto *config = new EffectConfig(effectFactory, &letters, mirror, isModifier);
                whole.applyConfig(config);
                delete config;
            } else {
                auto *config = new EffectConfig(effectFactory, nullptr, mirror, isModifier);
                whole.applyConfig(config);
                delete config;
            }
            break;
    }
}

void Display::clearModifier(const Scope scope) {
    switch (scope) {
        case SCOPE_LETTER:
            letters.clearModifier();
            break;
        case SCOPE_WORD:
            words.clearModifier();
            break;
        case SCOPE_WHOLE:
            whole.clearModifier();
            break;
    }
}

void Display::render() {
    switch (currentScope) {
        case SCOPE_LETTER:
            letters.render(allLeds, effectBufferArray, modifierBufferArray);
            break;

        case SCOPE_WORD:
            words.render(allLeds, effectBufferArray, modifierBufferArray);
            break;

        case SCOPE_WHOLE:
            whole.render(allLeds, effectBufferArray, modifierBufferArray);
            break;
    }

    alignSections();
    FastLED.show();
}

void Display::alignSections() {
    for (int i = 1; i < stripReversalSections.size(); i += 2) {
        auto section = stripReversalSections.at(i);
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
    delete[] effectBufferArray;
    delete[] modifierBufferArray;
}

Display *initDisplay(int brightness) {
    std::vector<Section> letters;
    std::vector<Section> words;
    std::vector<Section> stripReversalSection;
    int actualBrightness;

    if (IS_PROD) {
        letters = std::vector<Section>(
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
                });
        words = std::vector<Section>(
                {
                        Section(0, 74),
                        Section(75, 82),
                        Section(83, 135),
                        Section(136, 143),
                        Section(144, 190)
                });

        stripReversalSection = std::vector<Section>({});
        actualBrightness = brightness == -1 ? 30 : brightness;
    } else {
        letters = std::vector<Section>(
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
                });

        words = std::vector<Section>(
                {
                        Section(0, 63),
                        Section(64, 175),
                        Section(176, 255)
                });

        stripReversalSection = std::vector<Section>(
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

        actualBrightness = brightness == -1 ? 1 : brightness;
    }

    return new Display(
            Cluster(letters, SCOPE_LETTER),
            Cluster(words, SCOPE_WORD),
            stripReversalSection,
            actualBrightness
    );
}
