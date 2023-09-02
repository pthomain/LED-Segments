// The smallest addressable group of LEDs is a Pixel, which abstracts a group of LEDs and can be assigned a single colour and luminosity value
// An object implementing the Styleable interface can be assigned an individual Style, composed of a Pattern (colours) and/or Effect (luminosity) that would apply to its range of Pixels
// A Section is composed of Pixels and implements Styleable.
// A Display is composed of Sections and implements Styleable.
// Each Section in a Display inherits the style of its parent by default however any style applied to an individual Section overrides its parent's value.
