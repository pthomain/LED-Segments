#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "engine/render/renderer.h"
#include "memory"

class SimpleRenderer : public Renderer {

private:
    CRGB *effectArray;
    std::unique_ptr<Effect> currentEffect = nullptr;
    uint16_t frameIndex = 0;

public :

    explicit SimpleRenderer(
            const DisplaySpec &displaySpec,
            const String &name
    );

    bool hasEffect() override { return currentEffect != nullptr; }

    void changeEffect(std::unique_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    ~SimpleRenderer() override {
        delete[] effectArray;
    }

};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
