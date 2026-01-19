[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# DisplaySpec

The `DisplaySpec` class is the bridge between the generic LED-Segments engine and your specific hardware project. It is an abstract base class that you must inherit from.

## Responsibilities

1.  **Hardware Definition**: Which pin is the data pin? What is the color order? How many LEDs?
2.  **Topology Definition**: How many logical "Segments" do you have? How long is each segment?
3.  **Mapping Logic**: Which physical LED corresponds to "Segment 0, Pixel 5"?

## Creating a Custom Spec

You typically create a header file (e.g., `MyProjectSpec.h`) and define a class that inherits from `DisplaySpec`.

### 1. Basic Configuration

You must define `LED_PIN` and `RGB_ORDER` as static constants. These are used by the template system to configure FastLED at compile time.

```cpp
class MySpec : public DisplaySpec {
public:
    static constexpr int LED_PIN = 5;
    static constexpr EOrder RGB_ORDER = GRB;
    
    // ...
};
```

### 2. Constructor & LayoutConfig

Your constructor must pass a `LayoutConfig` object to the base `DisplaySpec` constructor. This config object tells the engine which effects are allowed and how often they should appear.

```cpp
MySpec() : DisplaySpec(MyLayoutConfig) {}
```

### 3. Physical & Logical Dimensions

Implement these pure virtual methods:

```cpp
// Total number of physical LEDs
uint16_t nbLeds() const override { return 100; }

// How many logical segments in this layout?
uint16_t nbSegments(uint16_t layoutId) const override { 
    if (layoutId == 0) return 1; // One long strip
    return 2; // Two strips of 50
}

// How many pixels in a specific segment?
uint16_t segmentSize(uint16_t layoutId, uint16_t segmentIndex) const override {
    if (layoutId == 0) return 100;
    return 50;
}
```

### 4. The Mapping Function (`mapLeds`)

This is the most important function. It translates a logical coordinate `(segment, pixel)` into a physical `ledIndex`.

```cpp
void mapLeds(
    uint16_t layoutId,
    uint16_t segmentIndex,
    uint16_t pixelIndex,
    fract16 progress, // 0-65535, represents the frame's progress (rarely used for static mapping)
    const std::function<void(uint16_t)> &onLedMapped // Callback
) const override {
    
    uint16_t physicalIndex;

    if (layoutId == 0) {
        // Simple 1:1 mapping
        physicalIndex = pixelIndex;
    } else {
        // Split mapping: Segment 0 is 0-49, Segment 1 is 50-99
        physicalIndex = (segmentIndex * 50) + pixelIndex;
    }

    // You must call this callback with the calculated physical index
    onLedMapped(physicalIndex);
}
```

**Note**: You can call `onLedMapped` multiple times! This allows one logical pixel to light up multiple physical LEDs (e.g., for mirroring or low-resolution effects on high-density strips).

## Advanced: Multiple Layouts

The `layoutId` parameter allows you to define multiple "views" of your hardware.

**Example: 8x8 Matrix**
*   **Layout 0 (Rows)**: 8 segments of 8 pixels. Effects slide horizontally.
*   **Layout 1 (Columns)**: 8 segments of 8 pixels. Effects slide vertically.
*   **Layout 2 (Snake)**: 1 segment of 64 pixels. Effects run through the whole matrix.

The engine will randomly pick a layout when switching effects, keeping the display dynamic.

## Advanced: Polar Coordinates

If you are building a circular display or a radial pattern, you can override `toPolarCoords`. This is used by some advanced effects (like `SwirlEffect`) to map 2D patterns onto your strip.

```cpp
PolarCoords toPolarCoords(uint16_t pixelIndex) const override {
    // Return {angle, radius}
    // angle: 0-65535 (0 to 360 degrees)
    // radius: 0-65535 (0 to 1.0)
    return { ... };
}
```
