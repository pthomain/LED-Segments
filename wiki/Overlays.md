[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Overlays

Overlays are used to add additional effects on top of the base effect.

## Available Overlays

*   `NoOverlay`
*   `ChaseOverlay`
*   `DashOverlay`
*   `MatrixOverlay`
*   `MoireOverlay`
*   `SparkleOverlay`
*   `WaveOverlay`

## Example

```cpp
#include <ChaseOverlay.h>

ChaseOverlay overlay = {
    .color = {.r = 255, .g = 255, .b = 255},
    .width = 5,
};
```