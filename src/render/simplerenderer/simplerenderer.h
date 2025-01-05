#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "render/renderer.h"
#include "memory"

class SimpleRenderer : public Renderer {

private:
    CRGB *effectArray;
    std::unique_ptr<Effect> currentEffect = nullptr;

public :

    explicit SimpleRenderer(
            std::shared_ptr<DisplaySpec> displaySpec,
            const uint16_t effectArraySize
    );

    void changeEffect(std::unique_ptr<Effect> effect) override;

    void render(CRGB *outputArray) override;

    ~SimpleRenderer() override {
        delete[] effectArray;
    }

};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
