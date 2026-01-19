# LED-Segments

**A modern C++17 library for Arduino that abstracts LED strips into logical Segments and Pixels, enabling complex, layered effects on custom display shapes.**

## Why LED-Segments?

Directly addressing LEDs by index (e.g., `leds[i] = color`) works for simple strips, but becomes unmanageable for complex shapes (signs, wearables, matrices) or when you want to layer effects.

**LED-Segments** sits on top of **FastLED** to provide:

*   **Logical Mapping**: Group non-contiguous physical LEDs into logical "Segments".
*   **Layered Rendering**: Compose visuals dynamically: **Effect + Overlay + Transition**.
*   **Geometry Agnostic**: Write an effect once (e.g., a "Chase"), and run it on any layout (Linear, Matrix, Circle) just by changing the mapping.
*   **Procedural Generation**: The library doesn't just play a playlist. It procedurally generates unique "Scenes" by randomly combining **Layouts**, **Effects**, **Overlays**, **Palettes**, and **Mirrors**. A single "Rainbow" effect can look like a smooth gradient, a mirrored kaleidoscope, or a scattered sparkle explosion depending on the combination chosen by the engine.

## Key Concepts

1.  **DisplaySpec**: You define the hardware (pins) and the topology (how LEDs map to segments).
2.  **Layouts**: A single physical strip can have multiple logical layouts (e.g., treat a matrix as 8 rows OR as 1 continuous snake).
3.  **Renderables**:
    *   **Effects**: Base patterns (Rainbow, Noise, Plasma).
    *   **Overlays**: Modifiers on top (Sparkles, Glitch).
    *   **Transitions**: Smooth cross-fades between states.

## Quick Start

### 1. Define your Display Specification

Create a class inheriting from `DisplaySpec`. This tells the engine how your LEDs are wired.
In this example, we define an **8x16 Matrix** (8 rows, 16 columns) wired in a Zig-Zag pattern. We define **two layouts**:
1.  **Rows**: 8 horizontal segments of 16 pixels.
2.  **Columns**: 16 vertical segments of 8 pixels.

```cpp
// MyDisplaySpec.h
#include <LED-Segments.h>

using namespace LEDSegments;

class MyDisplaySpec : public DisplaySpec {
public:
    static constexpr int LED_PIN = 2;
    static constexpr EOrder RGB_ORDER = GRB;
    
    // Define Layout IDs for clarity
    enum Layouts { ROWS = 0, COLUMNS = 1 };

    // Pass your configuration to the base constructor
    MyDisplaySpec() : DisplaySpec(MyLayoutConfig) {}

    // Total LEDs: 8 rows * 16 columns = 128
    uint16_t nbLeds() const override { return 128; }

    // Define number of segments per layout
    uint16_t nbSegments(uint16_t layoutId) const override { 
        if (layoutId == ROWS) return 8;     // 8 Rows
        if (layoutId == COLUMNS) return 16; // 16 Columns
        return 0;
    }

    // Define size of each segment
    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override { 
        if (layoutId == ROWS) return 16;    // Each row is 16 pixels wide
        if (layoutId == COLUMNS) return 8;  // Each column is 8 pixels tall
        return 0;
    }

    // The Core Magic: Map logical (segment, pixel) to physical LED index
    void mapLeds(
        uint16_t layoutId, 
        uint16_t segmentIndex, 
        uint16_t pixelIndex, 
        fract16 progress, 
        const std::function<void(uint16_t)> &onLedMapped
    ) const override {
        
        uint16_t row, col;

        if (layoutId == ROWS) {
            // Logical: segment=row, pixel=col
            row = segmentIndex;
            col = pixelIndex;
        } else {
            // Logical: segment=col, pixel=row
            col = segmentIndex;
            row = pixelIndex;
        }

        // Convert (row, col) to physical index (Zig-Zag wiring)
        uint16_t physicalIndex;
        if (row % 2 == 0) {
            // Even rows run Left -> Right
            physicalIndex = (row * 16) + col;
        } else {
            // Odd rows run Right -> Left
            physicalIndex = (row * 16) + (15 - col);
        }

        onLedMapped(physicalIndex); 
    }
};
```

### 2. Run the Display

In your main sketch, instantiate the `Display` with your spec.

```cpp
#include "MyDisplaySpec.h"

// Create the display controller
std::unique_ptr<Display<MyDisplaySpec>> display;

void setup() {
    // Initialize
    display = std::make_unique<Display<MyDisplaySpec>>();
}

void loop() {
    // The display handles the render loop, effect switching, and transitions
    display->loop();
}
```

## Advanced Features

### Multiple Layouts
You can define multiple ways to view your LEDs. For the 8x16 matrix above, you could add a second Layout ID that treats the entire matrix as one giant 128-pixel spiral. The engine will randomly switch between "Row Mode" and "Spiral Mode," making the same effects look completely different.

### Segment Repetition
One of the most powerful features of **LED-Segments** is how it handles rendering across multiple segments. When an effect is rendered, the engine iterates through every segment defined in your current layout. It renders the effect for that specific segment and then maps it to the physical LEDs.

This means a single "Rainbow" effect isn't just stretched across your entire display. Instead, it is **repeated** for each segment.
*   If your layout defines **1 Segment** (the whole strip), you get one long rainbow.
*   If your layout defines **8 Segments** (e.g., rows of a matrix), you get 8 identical rainbows running in parallel.

By simply changing the Layout ID, you drastically change the visual output without modifying the effect code itself.

### Variable Segment Sizes
Segments within a single layout do not have to be the same length. This is particularly useful for:
*   **Concentric Rings**: Outer rings have more LEDs than inner rings.
*   **Irregular Shapes**: A display might have a long section and a short section.

The engine automatically adapts the effect rendering to the size of each individual segment.

### Mirrors & Modifiers
Effects can be automatically mirrored, reversed, or repeated without changing the effect code.
*   `Mirror::REVERSE`: Runs the effect backwards.
*   **Combinatorial Power**: An effect like "Slide" combined with `Mirror::CENTRE` creates a "pulse" from the middle. Combined with `Mirror::REPEAT`, it creates multiple moving segments.

## Installation

### PlatformIO (Recommended)
This library is fully compatible with PlatformIO and includes a `library.json` manifest for easy dependency management.

Add the following to your `platformio.ini`:

```ini
[env:myboard]
platform = ...
board = ...
framework = arduino
lib_deps = 
    https://github.com/pthomain/LED-Segments.git
    fastled/FastLED
build_flags = -std=c++17
```

### Arduino IDE
1.  Download the repository as a ZIP.
2.  Sketch -> Include Library -> Add .ZIP Library.
3.  Ensure you have a board package that supports **C++17**.

## Requirements
*   **C++17** compatible compiler/board (ESP32, Teensy, modern AVR cores).
*   **FastLED** library.
