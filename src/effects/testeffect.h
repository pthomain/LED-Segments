#pragma once

#ifndef PONGEFFECT_H
#define PONGEFFECT_H

#include "effect.h"
#include "utils.h"
#include "section.h"

//class TestEffect : public Effect, public EffectFactory<TestEffect> {
//
//private:
//    int position = 0;
//    int hue = 0;
//
//    void confetti(CRGB *targetArray);
//
//    void sinelon(CRGB *targetArray);
//
//    void bpm(CRGB *targetArray);
//
//    void juggle(CRGB *targetArray);
//
//    void rainbow(CRGB *targetArray);
//
//    void movingDots(CRGB *targetArray);
//
//    void rainbowBeat(CRGB *targetArray);
//
//    void redWhiteBlue(CRGB *targetArray);
//
//    void tricolourRainbow(CRGB *targetArray);
//
//public:
//
//    explicit TestEffect(Modifier *modifier) : Effect(modifier) {}
//
//    ~TestEffect() override = default;
//
//    void fillArray(CRGB *targetArray) override;
//
//    static const std::function<std::unique_ptr<Effect>(Modifier * )> factory;
//};

#endif //PONGEFFECT_H