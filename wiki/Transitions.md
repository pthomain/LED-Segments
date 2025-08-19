[Home](Home.md) | [DisplaySpec](DisplaySpec.md) | [Effects](Effects.md) | [Overlays](Overlays.md) | [Transitions](Transitions.md)

# Transitions

Transitions are used to smoothly transition between effects.

## Available Transitions

*   `NoTransition`
*   `FadeTransition`
*   `SlideTransition`

## Example

```cpp
#include <FadeTransition.h>

FadeTransition transition = {
    .duration = 1000,
};
```