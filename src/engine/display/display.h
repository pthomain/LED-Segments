#ifndef LED_SEGMENTS_DISPLAY_H
#define LED_SEGMENTS_DISPLAY_H

#include "FastLED.h"
#include "engine/effect/effect.h"
#include "engine/displayspec.h"
#include "engine/render/renderer.h"

class Display {
private:
    const uint8_t effectDurationsInSecs;
    const uint8_t fps;
    const int16_t transitionDurationInMillis;
    const uint16_t refreshRateInMillis;
    const DisplaySpec &displaySpec;
    const std::unique_ptr<Renderer> renderer;
    const std::vector<EffectFactory> effectFactories;
    CRGB *outputArray;

    explicit Display(
            CRGB *outputArray,
            const DisplaySpec &displaySpec,
            const std::vector<EffectFactory> effectFactories,
            const uint8_t brightness,
            const uint8_t effectDurationsInSecs,
            const uint8_t fps,
            const int16_t transitionDurationInMillis
    );

public:

    template<int LED_PIN, EOrder RGB_ORDER>
    static Display *create(
            const DisplaySpec &displaySpec,
            const std::vector<EffectFactory> effectFactories,
            const uint8_t brightness = 50,
            const uint8_t effectDurationsInSecs = 5,
            const uint8_t fps = 60,
            const int16_t transitionDurationInMillis = 1000 //use < 1 to disable
    ) {
        CRGB *outputArray = new CRGB[displaySpec.nbLeds()];
        CFastLED::addLeds<WS2812B, LED_PIN, RGB_ORDER>(outputArray, displaySpec.nbLeds());
        return new Display(
                outputArray,
                std::move(displaySpec),
                std::move(effectFactories),
                brightness,
                effectDurationsInSecs,
                fps,
                transitionDurationInMillis
        );
    }

    void changeEffect();

    void render();

    void loop();

    ~Display() {
        delete[] outputArray;
    }
};


#endif //LED_SEGMENTS_DISPLAY_H
