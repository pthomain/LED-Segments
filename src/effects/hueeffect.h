#pragma once

#ifndef HUEEFFECT_H
#define HUEEFFECT_H

#include "effects.h"
#include "utils.h"
#include "section.h"

class HueEffect : public Effect, public EffectFactory<HueEffect> {
private :
    int hue = 0;
    int position = 0;

    void rainbowWithGlitter(CRGB *targetArray);

    void addGlitter(CRGB *targetArray, fract8 chanceOfGlitter);

    void confetti(CRGB *targetArray);

    void sinelon(CRGB *targetArray);

    void bpm(CRGB *targetArray);

    void juggle(CRGB *targetArray);

    void rainbow(CRGB *targetArray);

    void rgbGradient(CRGB *targetArray);

public:

    explicit HueEffect(Section section, Mirror mirror) : Effect(section, mirror) {}

    ~HueEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static const std::function<std::unique_ptr<Effect>(Section, Mirror)> factory;
};

#endif //HUEEFFECT_H