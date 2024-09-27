#include "FastLED.h"
#include "utils/utils.h"
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
    stripReversalSections(std::move(stripReversalSections)),
    allLeds(new CRGB[totalLeds]),
    fader(allLeds, totalLeds) {
    delay(2000);

    set_max_power_in_volts_and_milliamps(5, 500);
    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(allLeds, totalLeds);
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
    FastLED.show();
}

void Display::pickNewEffect(
        const uint16_t transitionDurationInFrames,
        const std::vector<std::pair<std::function<Effect *(
                const EffectContext &effectContext)>, Variation> > &effectFactories
) {
    const auto &effectFactoryPair = effectFactories.at(random8(effectFactories.size()));
    const auto &effectFactory = effectFactoryPair.first;
    const Variation &variation = effectFactoryPair.second;

    auto scope = SCOPE_WHOLE;
    auto pixelUnit = UNIT_PIXEL;

    const auto &scopeAndUnits = variation.scopeAndUnits;
    const auto &scopeAndUnit = scopeAndUnits.at(random8(scopeAndUnits.size()));
    scope = scopeAndUnit.first;
    pixelUnit = scopeAndUnit.second;

    const auto &mirrors = variation.mirrors;
    const auto mirror = mirrors.at(random8(mirrors.size()));

    std::function<Effect *(const EffectContext &effectContext)> *modifierFactory = nullptr;

    const auto &modifierFactories = variation.modifierFactories;

    //TODO slow modifier speed and take mirror into account
    bool hasModifier = !(
            (scope == SCOPE_WHOLE && pixelUnit == UNIT_PIXEL) //too small
            || (scope == SCOPE_WORD && pixelUnit == UNIT_PIXEL) //too small
            || (scope == SCOPE_WHOLE && pixelUnit == UNIT_WORD) //too big
            || (scope == SCOPE_WORD && pixelUnit == UNIT_LETTER) //too big
    );

    if (hasModifier && !modifierFactories.empty()) {
        modifierFactory = modifierFactories.at(random8(modifierFactories.size()));
    }

    Cluster *pixelUnits = nullptr;
    Cluster *scopeCluster = nullptr;

    switch (scope) {
        case SCOPE_LETTER:
            scopeCluster = &letters;
            break;
        case SCOPE_WORD:
            scopeCluster = &words;
            if (pixelUnit == UNIT_LETTER) pixelUnits = &letters;
            break;
        case SCOPE_WHOLE:
            scopeCluster = &whole;
            if (pixelUnit == UNIT_LETTER)
                pixelUnits = &letters;
            else if (pixelUnit == UNIT_WORD)
                pixelUnits = &words;
            break;
    }

    std::string output = scopeToString(scope)
                         + "\t" + pixelUnitToString(pixelUnit)
                         + "\t" + mirrorToString(mirror);
    Serial.println(output.c_str());

    fader.applyConfig(
            new EffectConfig(
                    *scopeCluster,
                    effectFactory,
                    modifierFactory,
                    pixelUnits,
                    mirror
            ),
            transitionDurationInFrames
    );
}

void Display::render() {
    fader.render();
    alignSections();
    FastLED.show();
}

void Display::alignSections() const {
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

Display *initDisplay(uint8_t brightness) {
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

        actualBrightness = brightness == -1 ? 10 : brightness;
    }

    return new Display(
            Cluster(letters, SCOPE_LETTER),
            Cluster(words, SCOPE_WORD),
            stripReversalSection,
            actualBrightness
    );
}
