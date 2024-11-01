#ifndef LED_SEGMENTS_DISPLAYBUILDER_H
#define LED_SEGMENTS_DISPLAYBUILDER_H

#include "DisplayType.h"
#include "displays/display/display.h"

class DisplayBuilder {

public :

    explicit DisplayBuilder() = default;

    Display *build(DisplayType displayType);
};

#endif //LED_SEGMENTS_DISPLAYBUILDER_H
