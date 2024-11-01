#ifndef LED_SEGMENTS_DLHDISPLAY_H
#define LED_SEGMENTS_DLHDISPLAY_H

#include "displays/display/display.h"

Segment *createLetter(uint16_t start, uint16_t end);

Display *dlhDisplay() {
    std::vector<Segment *> layouts;

    auto d = createLetter(0, 12);
    auto a = createLetter(13, 25);
    auto n = createLetter(26, 41);
    auto c = createLetter(42, 56);
    auto e = createLetter(57, 74);

    auto dance = new Segment(std::vector<Pixel *>({d, a, n, c, e}));

    auto colon1 = new Segment(std::vector<Pixel *>({createLetter(75, 82)}));

    auto l = createLetter(83, 93);
    auto o = createLetter(94, 104);
    auto v = createLetter(105, 117);
    auto e2 = createLetter(118, 135);

    auto love = new Segment(std::vector<Pixel *>({l, o, v, e2}));

    auto colon2 = new Segment(std::vector<Pixel *>({createLetter(136, 143)}));

    auto h = createLetter(144, 161);
    auto u = createLetter(162, 177);
    auto b = createLetter(178, 190);

    auto hub = new Segment(std::vector<Pixel *>({h, u, b}));

    auto root = new Segment(
            std::vector<Pixel *>(
                    {
                            dance,
                            colon1,
                            love,
                            colon2,
                            hub
                    }
            )
    );

    layouts.emplace_back(root);

    return new Display(190, layouts);
}

Segment *createLetter(uint16_t start, uint16_t end) {
    auto pixels = std::vector<Pixel *>();

    for (uint16_t i = start; i <= end; i++) {
        pixels.emplace_back(new Pixel(i, i));
    }

    return new Segment(pixels);
}

#endif //LED_SEGMENTS_DLHDISPLAY_H
