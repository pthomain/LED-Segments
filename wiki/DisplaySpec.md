[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# DisplaySpec

The `DisplaySpec` class is an abstract class that defines the physical layout of your LEDs and how they are mapped to virtual pixels. You need to create a subclass of `DisplaySpec` and implement its pure virtual functions.

## Pure Virtual Functions

### `uint16_t nbLeds() const`

This function should return the total number of LEDs in your display.

### `uint16_t nbSegments(uint16_t layoutId) const`

This function should return the number of segments for a given layout. A layout is a way of grouping virtual pixels.

### `uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const`

This function should return the size of a segment for a given layout. The size is the number of virtual pixels in the segment.

### `void mapLeds(uint16_t layoutId, uint16_t segmentIndex, uint16_t pixelIndex, float progress, const std::function<void(uint16_t)> &onLedMapped) const`

This function is the core of the `DisplaySpec`. It maps a virtual pixel to one or more physical LEDs.

*   `layoutId`: The current layout ID.
*   `segmentIndex`: The index of the segment within the layout.
*   `pixelIndex`: The index of the virtual pixel within the segment.
*   `progress`: The progress of the current effect, which can be used for dynamic mappings.
*   `onLedMapped`: A callback function that you should call for each physical LED that corresponds to the given virtual pixel.

## Example

Here is an example of a `DisplaySpec` for a simple 8x32 matrix display:

```cpp
#include "engine/displayspec/DisplaySpec.h"

class MatrixDisplaySpec : public DisplaySpec {
public:
    static constexpr int LED_TYPE = WS2812B;
    static constexpr int LED_PIN = 9;
    static constexpr EOrder RGB_ORDER = GRB;

    explicit MatrixDisplaySpec(): DisplaySpec(...) {}

    uint16_t nbLeds() const override { return 256; }

    uint16_t nbSegments(uint16_t layoutId) const override {
        // ...
    }

    uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override {
        // ...
    }

    void mapLeds(
        uint16_t layoutId,
        uint16_t segmentIndex,
        uint16_t pixelIndex,
        float progress,
        const std::function<void(uint16_t)> &onLedMapped
    ) const override {
        // ...
    }
};
```