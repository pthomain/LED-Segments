#ifndef LED_SEGMENTS_TESTDISPLAY_H
#define LED_SEGMENTS_TESTDISPLAY_H

#include "displays/display/display.h"
#include <cstdint>
#include <vector>

static Display *createDisplay() {
    auto rows = std::vector<Pixel *>();

    printNumber("before", freeMemory());

    //TODO running out of memory here with 256 pixels
    for (int i = 0; i < 32; i++) {
        auto row = std::vector<Pixel *>();
        for (int j = 0; j < 8; j++) {
            uint16_t index = i % 2 == 0 ? i * 32 + j : i * 32 + 7 - j;
            row.push_back(new Pixel(index, index));
        }
        rows.push_back(new Segment(row));
    }

    printNumber("after", freeMemory());

    Segment *root = new Segment(std::vector<Pixel *>(rows));

    return new Display(256, std::vector<Segment *>({root}));
}

#endif //LED_SEGMENTS_TESTDISPLAY_H
