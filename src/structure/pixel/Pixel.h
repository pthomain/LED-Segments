#pragma once

#ifndef LED_SEGMENTS_PIXEL_H
#define LED_SEGMENTS_PIXEL_H

#include <cstdint>
#include <utility>
#include <vector>
#include "crgb.h"

/**
 * Smallest unit to display a colour, can contain 1 or more LEDs.
 */
class Pixel {

protected:
    std::vector<uint16_t> _ledIndexes;

public:

    explicit Pixel(uint16_t start, uint16_t end);

    explicit Pixel(const std::vector<Pixel *> &pixels);

    explicit Pixel(const std::vector<uint16_t> &ledIndexes) : _ledIndexes(ledIndexes) {};

    virtual ~Pixel() = default; // Adding a virtual destructor to make the class polymorphic

    std::vector<uint16_t> ledIndexes() { return _ledIndexes; }

    void setColour(CRGB *output, CRGB colour);
};

#endif //LED_SEGMENTS_PIXEL_H
