#include "Pixel.h"
#include <algorithm>
#include <cstdint>

Pixel::Pixel(uint16_t start, uint16_t end) {
    this->_start = std::min(start, end);
    this->_end = std::max(start, end);
}
