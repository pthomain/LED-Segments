#ifndef LED_SEGMENTS_SEGMENT_H
#define LED_SEGMENTS_SEGMENT_H

#include "Pixel.h"
#include <cstdint>
#include <vector>
#include <utility> // For std::pair

// Collection of Pixels. This structure is recursive, i.e. a Segment can be composed of other Segments.
// When applying an effect, a depth parameter is provided, which identifies which level of recursion should
// be considered for repetition. That means that the effect will be repeatedly applied to each individual sub-Segment
// all the way down to the depth level, at which point the sub-Segments will be considered to be Pixels.
// The depth at which one of the Pixels is not a Segment is the bottom.
// The depth of a Segment is also the depth of its shortest branch
class Segment : public Pixel {

private:
    std::vector<Pixel *> pixels;
    uint8_t _depth;

    uint8_t getDepth(uint8_t depth);

    static std::pair<uint16_t, uint16_t> getSegmentRange(const std::vector<Pixel *> pixels);

public:

    explicit Segment(std::vector<Pixel *> pixels)
            : Pixel(getSegmentRange(pixels)), pixels(pixels) {
        if(pixels.empty()) {
            throw std::invalid_argument("Segment must contain at least one Pixel");
        }
        _depth = getDepth(0);
    }

    uint8_t depth() {
        return _depth;
    }
};

#endif //LED_SEGMENTS_SEGMENT_H
