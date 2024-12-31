#include "simplerenderer.h"

SimpleRenderer::SimpleRenderer(const uint16_t effectArraySize) :
        Renderer(effectArraySize),
        effectArray(new CRGB[effectArraySize]) {
    for (uint16_t i = 0; i < effectArraySize; i++) {
        effectArray[i] = CRGB::Black;
    }
}

void SimpleRenderer::changeEffect(
        std::shared_ptr<Effect> effect,
        const std::vector<Segment *> &layout
) {
    currentEffect = effect;
    this->layout = layout;
}

void SimpleRenderer::render(CRGB *outputArray) {
    for (const auto &segment: layout) {
        currentEffect->fillArray(effectArray, segment->size);
        segment->map(effectArray, outputArray);
    }
}
