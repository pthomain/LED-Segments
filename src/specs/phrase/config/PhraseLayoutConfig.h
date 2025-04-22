#ifndef PHRASELAYOUTCONFIG_H
#define PHRASELAYOUTCONFIG_H

#include <effects/noise/NoiseEffect.h>
#include <effects/party/PartyEffect.h>
#include <effects/rainbow/RainbowEffect.h>

// Format is PIXELS_IN_SEGMENTS
enum PhraseLayout {
    LEDS_IN_LETTERS,

    LETTERS_IN_WORDS,
    LEDS_IN_WORDS,

    WORDS_IN_WHOLE,
    LETTERS_IN_WHOLE,
    LEDS_IN_WHOLE
};

static constexpr uint16_t NB_LAYOUTS = 6;

static const std::vector<uint16_t> pixelHeavyLayouts = std::vector<uint16_t>{
    2, //LEDS_IN_WORDS,
    5 //LEDS_IN_WHOLE
};

static const std::vector<uint16_t> balancedLayouts = std::vector<uint16_t>{
    0, //LEDS_IN_LETTERS,
    1, //LETTERS_IN_WORDS,
    4, //LETTERS_IN_WHOLE,
};

static const std::vector<uint16_t> segmentHeavyLayouts = std::vector<uint16_t>{
    3 //WORDS_IN_WHOLE,
};

static const std::vector<uint16_t> mirrorableLayouts = std::vector<uint16_t>{
    0, //LEDS_IN_LETTERS,
    2, //LEDS_IN_WORDS,
    4, //LETTERS_IN_WHOLE,
    5 //LEDS_IN_WHOLE
};

//Prevents instantiation before setup() is called
inline const LayoutCatalog &phraseLayoutCatalog() {
    static const auto catalog = LayoutCatalog(
        NB_LAYOUTS,
        pixelHeavyLayouts,
        balancedLayouts,
        segmentHeavyLayouts,
        mirrorableLayouts,
        balancedLayouts,
        segmentHeavyLayouts,
        std::vector{
            NoiseEffect::factory,
            PartyEffect::factory,
            RainbowEffect::factory
        },
        NO_EFFECTS
    );
    return catalog;
}

static const std::vector<EffectFactory> supportedEffects = std::vector{
    // NoiseEffect::factory,
    // PartyEffect::factory,
    RainbowEffect::factory
};

#endif //PHRASELAYOUTCONFIG_H
