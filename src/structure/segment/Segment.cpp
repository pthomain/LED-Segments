#include "Segment.h"
#include <algorithm>
#include <cstdint>


Segment::Segment(const std::vector<Pixel *> &pixels) : Pixel(pixels), pixels(pixels) {
    if (pixels.empty()) {
        throw std::invalid_argument("Segment must contain at least one Pixel");
    }
    _bottom = getBottom(0);

    _ledIndexes = std::vector<uint16_t>();

    for (Pixel *pixel: pixels) {
        _ledIndexes.insert(_ledIndexes.end(), pixel->ledIndexes().begin(), pixel->ledIndexes().end());
    }
}

uint8_t Segment::getBottom(uint8_t depth) {
    uint8_t minDepth = std::numeric_limits<uint8_t>::max();
    for (Pixel *pixel: pixels) {
        auto *segment = dynamic_cast<Segment *> (pixel);
        if (segment) {
            minDepth = std::min(minDepth, segment->getBottom(depth + 1));
        } else {
            return depth + 1;
        }
    }
    return minDepth;
}

std::vector<Pixel *> Segment::getCeilingSegments(uint8_t ceiling) {
    return getPixelsAtDepth(0, ceiling, pixels);
}

std::vector<Pixel *> Segment::getPixelsAtDepth(
        uint8_t currentDepth,
        uint8_t targetDepth,
        const std::vector<Pixel *> &pixelsAtCurrentDepth
) {
    std::vector<Pixel *> result;

    if (currentDepth == targetDepth) {
        result.insert(result.end(), pixelsAtCurrentDepth.begin(), pixelsAtCurrentDepth.end());
    } else {
        for (Pixel *pixel: pixelsAtCurrentDepth) {
            auto *segment = dynamic_cast<Segment *> (pixel);
            if (segment) {
                auto subPixels = getPixelsAtDepth(
                        currentDepth + 1,
                        targetDepth,
                        segment->pixels
                );

                result.insert(result.end(), subPixels.begin(), subPixels.end());
            }
        }
    }

    return result;
}

std::vector<std::vector<Pixel *>> Segment::slicePixels(uint8_t ceiling, uint8_t floor) {
    auto result = std::vector<std::vector<Pixel *>>();
    auto ceilingSegments = getCeilingSegments(ceiling);

    for (const auto ceilingSegment: ceilingSegments) {
        if (auto segment = dynamic_cast<Segment *>(ceilingSegment)) {
            result.push_back(getPixelsAtDepth(ceiling, floor, segment->pixels));
        }
    }

    return result;
}
