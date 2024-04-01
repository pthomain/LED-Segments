#pragma once

#ifndef PONGEFFECT_H
#define PONGEFFECT_H

#include "effects.h"
#include "utils.h"
#include "section.h"

class PongEffect : public Effect, public EffectFactory<PongEffect> {

private:
    int position = 0;
    bool isReversed = false;

public:

    explicit PongEffect(Section section) : Effect(section) {
        position = section.start;
    }

    ~PongEffect() override = default;

    void fillArray(CRGB *targetArray) override;

    static const std::function<std::unique_ptr<Effect>(Section)> factory;
};

#endif //PONGEFFECT_H