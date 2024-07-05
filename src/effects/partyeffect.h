#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"
#include "config/variation.h"

static const std::vector<CRGBPalette16> palettes = std::vector<CRGBPalette16>{
        PartyColors_p,
        ForestColors_p,
        CloudColors_p,
        OceanColors_p
};

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {
private:
    CRGBPalette16 palette;
    uint8_t paletteIndex = 0;

public:
    explicit PartyEffect(
            const Section &section,
            const Mirror mirror
    ) : Effect(section, mirror) {
        palette = palettes[paletteIndex]; //random8(0, palettes.size())];
        paletteIndex = (paletteIndex + 1) % palettes.size();
    }

    ~PartyEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror)> factory;
    static Variation variation;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
