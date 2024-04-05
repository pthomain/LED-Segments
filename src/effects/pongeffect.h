#pragma once

#ifndef PONGEFFECT_H
#define PONGEFFECT_H

#include "effects.h"
#include "utils.h"
#include "section.h"

class PongEffect : public Effect, public EffectFactory<PongEffect> {

private:
    int position = 0;
    int hue = 0;
    bool isReversed = false;

    void pong(CRGB *targetArray);

    void movingDots(CRGB *targetArray);

    void rainbowBeat(CRGB *targetArray);

    void redWhiteBlue(CRGB *targetArray);

    void tricolourRainbow(CRGB *targetArray);

    void party(CRGB *targetArray);

public:

    explicit PongEffect(
            Section section,
            Mirror mirror
    ) : Effect(section, mirror) {
        position = section.start;
    }

    ~PongEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static const std::function<std::unique_ptr<Effect>(Section, Mirror)> factory;
};

#endif //PONGEFFECT_H