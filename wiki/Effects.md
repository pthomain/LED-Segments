[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Effects

Effects are the primary visual component in LED-Segments. They are responsible for generating the base color data for your segments.

## How Effects Work

An effect is a class that implements the `Renderable` interface. Its main job is to fill a buffer of `CRGB` colors for a given segment.

1.  **Initialization**: The effect is created with a `RenderableContext` containing the palette, duration, and random parameters.
2.  **Rendering**: Every frame, `fillSegmentArray` is called. The effect calculates colors based on time, position, and its internal logic.
3.  **Mirroring**: The engine automatically handles mirroring (Reverse, Center, etc.) *before* or *after* the effect renders, depending on the configuration.

## Built-in Effects

The library comes with a suite of standard effects:

### 1. Linear Effects
These effects treat the segment as a 1D line.

*   **`GradientEffect`**: Displays a moving gradient based on the current palette.
*   **`SlideEffect`**: Moves solid blocks or soft shapes of color along the strip.
*   **`ChaosEffect`**: Random, chaotic color fluctuations.
*   **`NoiseEffect`**: Uses Perlin noise to create smooth, organic flowing colors (like clouds or lava).

### 2. Geometric Effects
These effects use 2D or polar coordinates (if supported by your `DisplaySpec`).

*   **`SwirlEffect`**: A rotating spiral pattern.
*   **`RoseEffect`**: Generates mathematical rose patterns (rhodonea curves).
*   **`FractalEffect`**: Renders fractal-like patterns (e.g., Mandelbrot-inspired).

## Configuration & Parameters

Effects are highly configurable. Instead of hardcoding values, the engine passes a map of parameters (`Params`) to the effect constructor. These parameters are randomly selected based on ranges defined in your `LayoutConfig`.

**Common Parameters:**
*   `SPEED`: How fast the effect moves.
*   `SCALE`: The "zoom" level of the pattern.
*   `DENSITY`: How "busy" the effect is.

## Creating a Custom Effect

To create a new effect, inherit from `Renderable<CRGB>`.

```cpp
class MyCustomEffect : public Renderable<CRGB> {
public:
    // Define parameter keys
    static const uint8_t MY_PARAM = 0;

    // Define default parameter values
    static Params declareParams() {
        return { {MY_PARAM, 100} };
    }

    // Constructor
    explicit MyCustomEffect(const RenderableContext &context) : Renderable(context) {}

    // The Name
    static const char *name() { return "MyCustomEffect"; }

    // The Render Loop
    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) override {
        
        // Get a parameter value
        uint16_t myValue = context.parameters.at(MY_PARAM);

        // Draw something
        for (int i = 0; i < segmentSize; i++) {
            segmentArray[i] = CRGB::Red;
        }
    }
    
    // Optional: Pre-frame logic
    void beforeFrame(fract16 progress, unsigned long timeElapsedInMillis) override {}
    void afterFrame(fract16 progress, unsigned long timeElapsedInMillis) override {}
    
    // Define supported operations (usually just EFFECT)
    WeightedOperations renderableOperations() override {
        return just(RenderableOperation::EFFECT);
    }
};
```
