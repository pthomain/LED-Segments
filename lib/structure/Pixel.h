#pragma once

#ifndef LED_SEGMENTS_PIXEL_H
#define LED_SEGMENTS_PIXEL_H

#include <vector>
#include <cstdint>
#include <utility> // For std::pair

// Smallest unit to display a colour, can contain 1 or more contiguous LEDs
class Pixel {

private:
    uint16_t _start;
    uint16_t _end;

public:

    explicit Pixel(uint16_t start, uint16_t end);

    explicit Pixel(std::pair<uint16_t, uint16_t> range) : Pixel(range.first, range.second) {};

    virtual ~Pixel() = default; // Adding a virtual destructor to make the class polymorphic

    uint16_t start() const {
        return _start;
    }

    uint16_t end() const {
        return _end;
    }
};

#endif //LED_SEGMENTS_PIXEL_H
