#ifndef LED_SEGMENTS_TESTDISPLAY_H
#define LED_SEGMENTS_TESTDISPLAY_H

#include <cstdint>
#include "utils/utils.h"
#include <vector>
#include "BitArray.h"
#include "structure/display.h"

static const uint16_t TOTAL_LEDS = 256;
static const uint8_t NB_ROWS = 32;
static const uint8_t NB_COLS = 8;
static const uint8_t LED_INDEX_RES = 8; // up to 256 LEDs

static Display *createDisplay() {
    printNumber("before createDisplay", freeMemory());

    auto rows = std::vector<Segment *>(NB_ROWS);
    Segment *root = new Segment(0, rows);
    uint16_t pixelIndex = 0;

    for (uint8_t rowIndex = 0; rowIndex < NB_ROWS; rowIndex++) {
        auto pixels = new BitArray();
        pixels->begin(LED_INDEX_RES, NB_COLS);

        for (uint8_t columnIndex = 0; columnIndex < NB_COLS; columnIndex++) {
            uint16_t mappedIndex = columnIndex % 2 == 0
                                   ? columnIndex * NB_ROWS + rowIndex
                                   : (rowIndex * NB_ROWS) + (NB_COLS - 1) - columnIndex;

            pixels->set(pixelIndex, mappedIndex);
            pixelIndex++;
        }

        rows.push_back(new Segment(1, pixels));
    }

    printNumber("after createDisplay", freeMemory());

    auto layout = std::vector<Segment *>({root});
    return new Display(256, layout);
}

#endif //LED_SEGMENTS_TESTDISPLAY_H
