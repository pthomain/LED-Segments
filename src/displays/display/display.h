#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include "FastLED.h"
#include "utils/utils.h"
#include <utility>
#include <vector>
#include <functional>
#include <string>
#include "effects/effect.h"
#include "render/fader/fader.h"
#include "structure/segment/Segment.h"

/**
 * Represents the physical display, which can have multiple layouts (some displays only have one layout).
 * Here's an example of a matrix display which can support 2 layouts:
 *
 * Layouts
 * ├── Layout 1 Root (layout index 0, segment bottom 0)
 * │   └── Vertical segments (layout index 0, segment bottom 1)
 * │       └── Pixels (layout index 0, segment bottom 2)
 * └── Layout 2 Root (layout index 1, segment bottom 0)
 *     └── Horizontal Segments (layout index 1, segment bottom 1)
 *         └── Pixels (layout index 1, segment bottom 2)
 *
 *  When applying an effect, you must first specify which layout to use, then the bottom at which to apply the effect.
 *  See Segment.h for more details on how bottom works.
 */
class Display {
private:
    std::vector<Segment *> layouts;
    uint16_t totalLeds;
    CRGB *allLeds;
    Fader fader;

public:
    explicit Display(uint16_t totalLeds, std::vector<Segment *> layouts);

    void changeEffects(
            const uint16_t transitionDurationInFrames,
            const std::vector<std::function<Effect *(const EffectContext &effectContext)>> &effectFactories
    );

    void render();

    ~Display() {
        delete[] allLeds;
    }
};

#endif //DISPLAY_H
