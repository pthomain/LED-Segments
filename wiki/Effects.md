[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Effects

Effects are used to create animations on the LEDs.

## Available Effects

*   `NoEffect`
*   `GradientEffect`
*   `NoiseEffect`
*   `SlideEffect`
*   `SwirlEffect`

## Example

```cpp
#include <GradientEffect.h>

GradientEffect effect = {
    .colors = {
        {.r = 255, .g = 0, .b = 0},
        {.r = 0, .g = 255, .b = 0},
        {.r = 0, .g = 0, .b = 255},
    },
};
```