#include "DisplayBuilder.h"
#include "../FibonacciDisplay.h"
#include "../DlhDisplay.h"
#include "../TestDisplay.h"
#include "displays/display/display.h"

Display *DisplayBuilder::build(DisplayType displayType) {
    switch (displayType) {
        case DisplayType::FIBONACCI:
            return fibonacciDisplay();

        case DisplayType::DLH:
            return dlhDisplay();

        default:
            return testDisplay();
    }
}
