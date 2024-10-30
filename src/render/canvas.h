#ifndef LED_SEGMENTS_CANVAS_H
#define LED_SEGMENTS_CANVAS_H

#include "FastLED.h"
#include "utils/utils.h"
#include <utility>
#include <vector>
#include "effects/effect.h"
#include "config/effectconfig.h"

class Canvas {
    CRGB *effectBufferArray = nullptr;
    CRGB *modifierBufferArray = nullptr;
    EffectConfig *currentEffectConfig = nullptr;

    uint16_t effectIteration = 0;
    uint16_t effectFrameIndex = 0;

    const std::vector<Section> emptySections = std::vector<Section>();

    std::vector<std::pair<Effect *, std::vector<Section>>> effectPerSectionPixels =
            std::vector<std::pair<Effect *, std::vector<Section>>>();

    std::vector<std::pair<Effect *, std::vector<Section>>> modifierPerSectionPixels =
            std::vector<std::pair<Effect *, std::vector<Section>>>();

    static std::vector<Section> intersectAllPixelsWithClusterScope(
        const Section &clusterSection,
        const std::vector<Section> &pixelSections
    );

    static void clearEffectOrModifier(std::vector<std::pair<Effect *, std::vector<Section>>> &effectMap);

    void applyEffectOrModifier(
        std::vector<std::pair<Effect *, std::vector<Section>>> &effectMap,
            const std::function<Effect *(const EffectContext &effectContext)> &effectFactory
    ) const;

public :
    void applyConfig(EffectConfig *effectConfig);

    explicit Canvas(const uint16_t totalLeds) {
        effectBufferArray = new CRGB[totalLeds];
        modifierBufferArray = new CRGB[totalLeds];
    };

    void render(CRGB *outputArray);

    ~Canvas() {
        delete[] effectBufferArray;
        delete[] modifierBufferArray;
    };
};

#endif //LED_SEGMENTS_CANVAS_H

