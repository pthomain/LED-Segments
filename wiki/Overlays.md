[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Overlays

Overlays are secondary visual layers that are rendered *on top* of the base Effect. They add texture, movement, and complexity to otherwise simple patterns.

## How Overlays Work

Like Effects, Overlays are `Renderable` objects. However, they interact with the existing color buffer in different ways defined by `RenderableOperation`.

### Blending Modes

Overlays can combine with the base effect using:

1.  **`OVERLAY_SOURCE`**: Replaces the underlying pixel (opaque).
2.  **`OVERLAY_SCREEN`**: Lightens the underlying pixel (like Photoshop's Screen mode). Good for sparkles.
3.  **`OVERLAY_MULTIPLY`**: Darkens the underlying pixel. Good for shadows or masking.
4.  **`OVERLAY_INVERT`**: Inverts the color of the underlying pixel.

## Built-in Overlays

*   **`SparkleOverlay`**: Random pixels flash white (or a specific color). Great for adding "glitter."
*   **`ChaseOverlay`**: A "comet" or "snake" that runs over the existing effect.
*   **`WaveOverlay`**: A sine-wave brightness modulation. Makes the strip "breathe."
*   **`MoireOverlay`**: High-frequency interference patterns.
*   **`MatrixOverlay`**: Falling code/rain effect (like The Matrix).
*   **`DashOverlay`**: Moving dashed lines (stippling).

## Usage Example

In your `LayoutConfig`, you define which overlays are allowed. The engine might pick:
*   **Base**: `RainbowEffect`
*   **Overlay**: `SparkleOverlay` (Screen Mode)

Result: A rainbow that glitters.

## Creating a Custom Overlay

It is almost identical to creating an Effect, but you typically return different `RenderableOperation` types.

```cpp
class MyMaskOverlay : public Renderable<CRGB> {
    // ... setup code ...

    void fillSegmentArray(
        CRGB *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress,
        unsigned long timeInMillis
    ) override {
        // Create a moving hole
        int pos = (timeInMillis / 10) % segmentSize;
        segmentArray[pos] = CRGB::Black; 
    }

    WeightedOperations renderableOperations() override {
        // This overlay will MULTIPLY the base effect
        // Since we drew Black (0,0,0), it will mask out that pixel.
        return just(RenderableOperation::OVERLAY_MULTIPLY);
    }
};
```
