#ifndef LED_SEGMENTS_SEGMENT_H
#define LED_SEGMENTS_SEGMENT_H

#include "FastLED.h"
#include <cstdint>
#include "vector"
#include "BitArray.h"
#include "effects/effect.h"

class Segment {

    //Each section has at least 1 pixel.
    std::vector<BitArray> pixels; // each effect calculate one colour per pixel

public:

    const uint16_t size;

    explicit Segment(const std::vector<std::vector<uint16_t>> pixels);

    void map(CRGB *effectArray, CRGB *outputArray);
};

#endif //LED_SEGMENTS_SEGMENT_H
