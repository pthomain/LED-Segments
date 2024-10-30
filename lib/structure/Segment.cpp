#include "Segment.h"
#include <algorithm>
#include <cstdint>
#include <utility> // For std::pair

uint8_t Segment::getDepth(uint8_t depth) {
    uint8_t minDepth = std::numeric_limits<uint8_t>::max();  // Initialize to max possible depth
    for (Pixel *pixel: pixels) {
        auto *segment = dynamic_cast<Segment *> (pixel);
        if (segment) {
            minDepth = std::min(minDepth, segment->getDepth(depth + 1));
        } else {
            return depth + 1;
        }
    }
    return minDepth;
}

std::pair<uint16_t, uint16_t> Segment::getSegmentRange(const std::vector<Pixel *> pixels) {
    uint16_t start = 0;
    uint16_t end = 0;
    for (const Pixel *pixel: pixels) {
        start = std::min(start, pixel->start());
        end = std::max(end, pixel->end());
    }
    return {start, end};
}