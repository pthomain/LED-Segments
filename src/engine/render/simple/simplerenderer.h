#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "engine/render/renderer.h"
#include "engine/render/pixelmapper.h"

class SimpleRenderer : public Renderer {
    CRGB *effectArray;
    std::shared_ptr<Effect> currentEffect = nullptr;
    uint16_t frameIndex = 0;

    //TODO unique ptr
    PixelMapper *pixelMapper;

public :
    explicit SimpleRenderer(
        const DisplaySpec &displaySpec,
        PixelMapper *pixelMapper,
        const String &name
    );

    // bool hasEffect() override {
    //     return currentEffect != nullptr;
    // }

    void changeEffect(std::shared_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    std::shared_ptr<Effect> getEffect() override;

    ~SimpleRenderer() override {
        delete[] effectArray;
        effectArray = nullptr;
        if (currentEffect) currentEffect.reset();
        pixelMapper = nullptr;
    }
};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
