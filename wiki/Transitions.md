[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Transitions

Transitions handle the visual switch from one "Scene" (Effect + Overlay) to the next. Instead of a hard cut, the library smoothly blends the two states.

## How Transitions Work

The `Renderer` maintains two full render pipelines:
1.  **Current Scene**: The effect that is fading out.
2.  **Next Scene**: The effect that is fading in.

A `Transition` is a `Renderable<uint8_t>` (note: it returns bytes, not colors). It generates a **mask** (0-255) that determines how much of the "Next Scene" is visible.

*   **0**: Show Current Scene completely.
*   **255**: Show Next Scene completely.
*   **128**: 50/50 blend.

## Built-in Transitions

*   **`FadeTransition`**: A simple global cross-fade. The entire strip goes from 0% to 100% opacity simultaneously.
*   **`SlideTransition`**: A "wipe" effect. The new scene slides in from one side, covering the old scene.
*   **`NoTransition`**: A hard cut.

## The Transition Lifecycle

1.  **Trigger**: The `Display` decides it's time to change effects.
2.  **Setup**: The "Next Scene" is initialized in the background.
3.  **Loop**:
    *   Both scenes are rendered to internal buffers.
    *   The `Transition` renderable fills a mask buffer.
    *   The `Renderer` blends the two buffers using the mask.
4.  **Completion**: Once the transition duration is over, the "Next Scene" becomes the "Current Scene," and the "Old Scene" is discarded to free memory.

## Creating a Custom Transition

Inherit from `Renderable<uint8_t>`.

```cpp
class CurtainTransition : public Renderable<uint8_t> {
    // ... setup ...

    void fillSegmentArray(
        uint8_t *segmentArray,
        uint16_t segmentSize,
        uint16_t segmentIndex,
        fract16 progress, // 0 to 65535 (0% to 100% complete)
        unsigned long timeInMillis
    ) override {
        
        // Calculate a "curtain" opening from the center
        uint16_t center = segmentSize / 2;
        uint16_t width = (segmentSize * progress) / 65535;
        
        for (int i = 0; i < segmentSize; i++) {
            int dist = abs(i - center);
            if (dist < width / 2) {
                segmentArray[i] = 255; // Show new scene
            } else {
                segmentArray[i] = 0;   // Show old scene
            }
        }
    }
};
```
