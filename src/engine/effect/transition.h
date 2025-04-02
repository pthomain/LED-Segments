#ifndef LED_SEGMENTS_TRANSITION_H
#define LED_SEGMENTS_TRANSITION_H

#include "crgb.h"
#include "mirror.h"

enum Transition {
    NONE,
    FADE,
    SLIDE_LTR,
    SLIDE_RTL,
    RANDOM
};

static void applyTransition(
        const Transition transition,
        const Mirror transitionMirror,
        uint8_t *transitionArray,
        const uint16_t segmentSize,
        const float transitionPercent
) {
    switch (transition) {
        case FADE:
        default:
            for (uint16_t i = 0; i < segmentSize; i++) {
                transitionArray[i] = transitionPercent * 255;
            }
            break;
    }
}

#endif //LED_SEGMENTS_TRANSITION_H
