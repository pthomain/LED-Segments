#ifndef LED_SEGMENTS_TRANSITION_H
#define LED_SEGMENTS_TRANSITION_H

#include <engine/mirror/Mirror.h>
#include "crgb.h"

enum Transition {
    NONE,
    FADE,
    SLIDE
};

const std::vector ALL_TRANSITIONS = {
    FADE,
    SLIDE
};

static void fillTransitionArray(
    const Transition transition,
    const Mirror transitionMirror,
    CRGB *transitionArray,
    const uint16_t segmentSize,
    const float transitionPercent
) {
    switch (transition) {
        case SLIDE: {
            uint16_t limit = segmentSize * transitionPercent;
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = i < limit ? CRGB::White : CRGB::Black;
            }
        }
        break;

        case FADE:
        default: {
            uint8_t alpha = 255 * transitionPercent;
            CRGB colour = CRGB(alpha, alpha, alpha);
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = colour;
            }
        }
            break;
    }
}

#endif //LED_SEGMENTS_TRANSITION_H
