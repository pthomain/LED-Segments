# LED Segments - An Arduino lib to abstract individual LEDs on a strip into virtual Pixels and Segments

This library provides a way to map an LED strip into one or more physical layouts. 
This is useful for managing effects on LED displays where different sections need to be controlled independently but where the LEDs are addressed by their linear indexes (e.g. a display sign where each letter is a section of the same strip).

Multiple layouts can be defined, if required, to allow for the display to be segmented in different ways while making it easy to switch from one layout to another during rendering.

Individual LEDs aren't addressed directly but are instead abstracted into Pixels and Segments:
- Each Layout is composed of at least one Segment and each Segment is composed of at least one Pixel.
- Each Pixel is itself composed of one or more LEDs.
- LED indexes do not have to be contiguous in a given Pixel, neither do Pixels need to be contiguous in a Segment.
  
Custom effects can be applied to any given Layout, where they are repeatedly applied to each Segment, using Pixel as the smallest addressable unit for applying a colour.
By randomising layouts in the rendering loop, you can achieve a multitude of different ways to render the same effect.
