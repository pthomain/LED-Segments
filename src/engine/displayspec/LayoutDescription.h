#ifndef LAYOUTDESCRIPTION_H
#define LAYOUTDESCRIPTION_H

#include <cstdint>
#include <vector>

enum LayoutDescription {
  PIXEL_HEAVY, // high ratio of pixels to segments, good for repeating patterns, too fine for transitions, useless for highlights
  BALANCED, // balanced ratio of pixels to segments, good for transitions and patterns, useless for highlights
  SEGMENT_HEAVY, // low ratio of pixels to segments, few colours, blocky transitions, good for highlights, no mirrors
  MIRRORABLE, // layouts that have enough pixels per segment to be mirrored
  TRANSITIONABLE, // layouts that are good for transitions
  HIGHLIGHTABLE // layouts that are good for highlights
};

#endif //LAYOUTDESCRIPTION_H
