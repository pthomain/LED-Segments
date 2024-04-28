#ifndef LED_MATRIX_DLH_PARTYEFFECT_H
#define LED_MATRIX_DLH_PARTYEFFECT_H

#include <vector>
#include "effect.h"
#include "colorutils.h"

static const std::vector<CRGBPalette16> palettes = std::vector<CRGBPalette16>{
        PartyColors_p,
        ForestColors_p,
        CloudColors_p,
        OceanColors_p
};

class PartyEffect : public Effect, public EffectFactory<PartyEffect> {

private:
    CRGBPalette16 palette;

public:
    explicit PartyEffect(
            const Section &section,
            const Mirror mirror
    ) : Effect(section, mirror) {
        palette = palettes[random8(0, palettes.size())];
    }

    ~PartyEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static std::function<Effect *(const Section &, const Mirror mirror)> factory;
};

#endif //LED_MATRIX_DLH_PARTYEFFECT_H
