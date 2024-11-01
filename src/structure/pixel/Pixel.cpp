#include "Pixel.h"
#include <cstdint>

Pixel::Pixel(uint16_t start, uint16_t end) {
    auto arraySize = end - start + 1;
    _ledIndexes = std::vector<uint16_t>(arraySize);

    for (uint16_t i = 0; i < arraySize; i++) {
        _ledIndexes[i] = start + i;
    }
}

Pixel::Pixel(const std::vector<Pixel *> &pixels) {
    uint16_t arraySize = 0;

    for (auto pixel: pixels) {
        arraySize += pixel->ledIndexes().size();
    }

    _ledIndexes = std::vector<uint16_t>(arraySize);

    uint16_t index = 0;
    for (auto pixel: pixels) {
        for (const auto &ledIndex: pixel->ledIndexes()) {
            _ledIndexes[index] = ledIndex;
            index++;
        }
    }
}

void Pixel::setColour(CRGB *output, CRGB colour) {
    for (const auto &ledIndex: _ledIndexes) {
        output[ledIndex] = colour;
    }
}
