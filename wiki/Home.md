[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# LED Segments

This is a library for driving individually addressable LEDs, such as WS2811, WS2812, SK6812, etc.

## Features

*   Customizable display layouts
*   Effects
*   Overlays
*   Transitions

## How it Works

The library works by creating a `Display` object, which is templated with a `DisplaySpec` class. The `DisplaySpec` class defines the physical layout of your LEDs and how they are mapped to virtual pixels.

The `Display` object manages a `Renderer`, which is responsible for rendering effects, overlays, and transitions. The `Display` object periodically calls a `changeEffect()` method to randomly select a new effect, overlay, and transition.

The `loop()` function in your sketch should call the `display->loop()` method, which will in turn call the `render()` method at a fixed rate to update the LEDs.

## Getting Started

To get started, you will need to create a class that inherits from `DisplaySpec` and implements its pure virtual functions. This class will define the layout of your LEDs.

Once you have your custom `DisplaySpec` class, you can create a `Display` object in your `setup()` function:

```cpp
#include <LED-Segments.h>
#include "MyDisplaySpec.h"

Display<MyDisplaySpec> *display;

void setup() {
    display = new Display<MyDisplaySpec>();
}

void loop() {
    display->loop();
}
```

## Examples

For examples, please see the `examples` directory.