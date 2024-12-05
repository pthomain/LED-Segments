#ifndef LED_SEGMENTS_FIBONACCIDISPLAY_H
#define LED_SEGMENTS_FIBONACCIDISPLAY_H

#include <cstdint>
#include <vector>
#include "displays/display/display.h"

static Display *createDisplay() {
    std::vector<Segment *> layouts;

    // Straight Fibonacci curve
    auto straightCurveSegments = std::vector<Pixel *>();

    for (uint8_t i = 0; i < 12; i++) {
        auto pixels = std::vector<Pixel *>();
        int start = i * 27;
        for (int j = start; j <= start + 14; j++) {
            pixels.emplace_back(new Pixel(i, j));
        }

        pixels.emplace_back(new Pixel(start + 15, start + 16));
        pixels.emplace_back(new Pixel(start + 17, start + 18));
        pixels.emplace_back(new Pixel(start + 19, start + 20));
        pixels.emplace_back(new Pixel(start + 21, start + 23));
        pixels.emplace_back(new Pixel(start + 24, start + 26));

        straightCurveSegments.push_back(new Segment(pixels));
    }

    auto straightCurves = std::vector<Pixel *>({straightCurveSegments});
    layouts.emplace_back(new Segment(straightCurves));

    // Bent Fibonacci curve
    auto bentCurveSegments = std::vector<Pixel *>();

    for (uint8_t i = 0; i < 12; i++) {
        auto pixels = std::vector<Pixel *>();

        for (uint8_t j = 0; j <= 16; j++) {
            Pixel *pixel = &(straightCurves[i][j]);
            pixels.emplace_back(pixel);
        }

        uint8_t adjacentPixel1 = (i - 1) % 12;
        uint8_t adjacentPixel2 = (i - 2) % 12;
        uint8_t adjacentPixel3 = (i - 3) % 12;
        pixels.emplace_back(&(straightCurves[adjacentPixel1][17]));
        pixels.emplace_back(&(straightCurves[adjacentPixel2][18]));
        pixels.emplace_back(&(straightCurves[adjacentPixel3][19]));

        bentCurveSegments.emplace_back(new Segment(pixels));
    }
    auto bentCurves = std::vector<Pixel *>({bentCurveSegments});
    layouts.emplace_back(new Segment(bentCurves));

//    // Vertical segments
//    auto oddVerticalSegments = std::vector<Pixel *>();
//    auto evenVerticalSegments = std::vector<Pixel *>();
//
//    for (uint8_t i = 0; i < 12; i++) {
//        int start = i * 27;
//        for (int i = start; i <= start + 14; i++) {
//            if (i % 2 == 0) {
//                evenVerticalSegments.emplace_back({i, i});
//            } else {
//                oddVerticalSegments.emplace_back({i, i});
//            }
//        }
//    }
//
//    layouts.push_back(new Segment(oddVerticalSegments));
//    layouts.push_back(new Segment(eventVerticalSegments));
//
//    // Concentric segments

    return new Display(324, layouts);
}

#endif //LED_SEGMENTS_FIBONACCIDISPLAY_H
