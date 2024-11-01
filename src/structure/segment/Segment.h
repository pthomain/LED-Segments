#ifndef LED_SEGMENTS_SEGMENT_H
#define LED_SEGMENTS_SEGMENT_H

#include "../pixel/Pixel.h"
#include <cstdint>
#include <vector>
#include <utility> // For std::pair
#include <stdexcept>

/**
 * Collection of Pixels. This structure is recursive, i.e. a Segment can be composed of other Segments.
 * The bottom of a Segment is the depth of its shortest branch, that is the minimum bottom at which the first true Pixel
 * is found on any of its sub-Segments.
 *
 * For example, consider the following Segment for a display representing 3 words of 5 letters each with each letter
 * made up of 10 Pixels (Pixels themselves can be made up of one or several LEDs, but are the smallest unit that can be
 * assigned a colour):
 *
 * Root (bottom 0)
 *  └── Words (bottom 1)
 *      └── Letters (bottom 2)
 *          └── Pixels (bottom 3 - bottom)
 *
 * Effects are applied as part of a set. Any effect set containing more than one effect must provide a floor
 * and a ceiling value. Any set containing a single effect only needs to provide a floor value (ceiling is 0 by default).
 *
 * For sets providing multiple effects, those are applied repeatedly to all sub-Segments found at the ceiling bottom.
 * The floor depth is the value indicating the max bottom (lesser or equal to the bottom) at which all sub-Segments
 * are considered as Pixels .
 *
 * In practice this means a single colour is applied to that sub-Segment and recursively to all of its Pixels.
 * This results in a scaling of the effect by effectively changing the pixel density.
 *
 * Effects in a effect set are padded using a modulo to cover all sub-Segments. To repeat the same effect, just include
 * it twice in the set.
 *
 * For example, imagine you want to apply a rainbow gradient and a greyscale gradient as part of the same effect set.
 *
 * If the ceiling depth is 1 and the floor bottom is 2, then the first word will have a rainbow gradient with each letter
 * being a different colour (5 colours in total). The second word will have a greyscale gradient with each letter being
 * a shade of grey (5 colours). Finally the third word will have the rainbow gradient again (5 colours).
 *
 * If the ceiling bottom is 1 and the floor depth is 3, then the first word will have a rainbow gradient with each Pixel
 * being a different colour (50 colours in total). The second word will have a greyscale gradient with each Pixel being
 * a shade of grey (50 colours). Finally the third word will have the rainbow gradient again (50 colours).
 *
 * If the ceiling depth is 2 and the floor bottom is 3, then every letter in every word will have a rainbow gradient with
 * each Pixel being a different colour (10 colours in total, 15 repetitions for each letter).
 *
 * Essentially the floor depth affects the pixel density while the ceiling bottom affects the scope of the repetition.
 * There is no repetition if the ceiling bottom is 0. Only a single colour per effect is created when providing
 * the same value for floor and ceiling depths.
 */
class Segment : public Pixel {

private:
    uint8_t _bottom;

    uint8_t getBottom(uint8_t depth);

    std::vector<Pixel *> getCeilingSegments(uint8_t ceiling);

    std::vector<Pixel *> getPixelsAtDepth(
            uint8_t currentDepth,
            uint8_t targetDepth,
            const std::vector<Pixel *> &pixelsAtCurrentDepth
    );

public:
    const std::vector<Pixel *> pixels;

    explicit Segment(const std::vector<Pixel *> &pixels);

    uint8_t bottom() { return _bottom; }

    std::vector<std::vector<Pixel *>> slicePixels(uint8_t ceiling, uint8_t floor);

};

#endif //LED_SEGMENTS_SEGMENT_H
