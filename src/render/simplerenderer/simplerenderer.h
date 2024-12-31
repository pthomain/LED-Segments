#ifndef LED_SEGMENTS_SIMPLERENDERER_H
#define LED_SEGMENTS_SIMPLERENDERER_H

#include "render/renderer.h"

class SimpleRenderer : public Renderer {

private:
    CRGB *effectArray;
    std::shared_ptr<Effect> currentEffect;
    std::vector<Segment *> layout;

public :

    explicit SimpleRenderer(const uint16_t effectArraySize);

    void changeEffect(
            std::shared_ptr<Effect> effect,
            const std::vector<Segment *> &layout
    ) override;

    void render(CRGB *outputArray) override;

};

#endif //LED_SEGMENTS_SIMPLERENDERER_H
