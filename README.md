# LED Segments - An Arduino lib to abstract individual LEDs on a strip into virtual Pixels and Segments

This library provides a way to map an LED strip into one or more physical layouts. 
This is useful for managing effects on LED displays where different sections need to be controlled independently but where the LEDs are addressed by their linear indexes (e.g. a display sign where each letter is a section of the same strip).

It provides a simple way to segment a display where the physical location of LEDs has little correlation to their addressable index, e.g. displays with complex shapes.

Multiple layouts can be defined, if required, to allow for the display to be segmented in different ways while making it easy to switch from one layout to another during rendering.

## Installation & Quick Start

### Option 1: Arduino IDE (Recommended for Beginners)

**Step 1: Install the Library**
1. Go to [LED-Segments GitHub](https://github.com/pthomain/LED-Segments)
2. Click **Code** → **Download ZIP**
3. In Arduino IDE: **Sketch** → **Include Library** → **Add .ZIP Library**
4. Select the downloaded ZIP file
5. Install **FastLED** library: **Tools** → **Manage Libraries** → Search "FastLED" → Install

**Step 2: Run the Example**
1. **File** → **Examples** → **LED-Segments** → **BasicUsage**
2. Connect your LED strip to the pin defined in `MatrixDisplaySpec.h` (default: pin 9)
3. Select your board: **Tools** → **Board**
4. Select your port: **Tools** → **Port**
5. Click **Upload** ⬆️

### Option 2: PlatformIO

**Method A: Use as Library Dependency (Recommended)**
1. Create a new PlatformIO project
2. Add to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/pthomain/LED-Segments.git
    fastled/FastLED@^3.10.0
build_flags = -std=c++17
```
3. Copy the example code from `examples/BasicUsage/` to your `src/main.cpp`
4. Run: `pio lib install` (installs dependencies)
5. Run: `pio run -t upload`

**Note**: Use `platformio.ini` configuration instead of `pio lib install LED-Segments` since this library is hosted on GitHub, not the PlatformIO Registry.

**Method B: Clone and Deploy Directly**
```bash
# Clone and enter directory
git clone https://github.com/pthomain/LED-Segments.git
cd LED-Segments

# Deploy to your board (modify platformio.ini to use BasicUsage as src)
# Note: This method requires manual configuration
```

## Hardware Setup

### Wiring
- **LED Strip Data Pin** → Arduino Pin 9 (or modify in `MatrixDisplaySpec.h`)
- **LED Strip Power** → External 5V power supply (for >30 LEDs)
- **LED Strip Ground** → Arduino Ground + Power supply Ground

### Supported Boards
- **Seeed Xiao** (default configuration)
- **Arduino Uno**
- **ESP32**
- Most Arduino-compatible boards with C++17 support

## Dependencies
- **FastLED** 3.10.0 or later
- **C++17** compatible compiler

## Usage

Individual LEDs aren't addressed directly but are instead abstracted into Pixels and Segments:
- Each Layout is composed of at least one Segment and each Segment is composed of at least one Pixel.
- Each Pixel is itself composed of one or more LEDs.
- LED indexes do not have to be contiguous in a given Pixel, providing flexibility to displays where you might want to group LEDs that are physically close together but far from each other in terms of their indexes on the strip.
  
Custom effects can be applied to any given Layout, where they are repeatedly applied to each Segment, using Pixel as the smallest addressable unit for applying a colour.
By randomising layouts in the rendering loop, you can achieve a multitude of different ways to render the same effect.

### Complete Working Example

The `examples/BasicUsage/` directory contains a fully functional example. Here's the basic structure:

```cpp
#include <LED-Segments.h>
#include "spec/MatrixDisplaySpec.h"

using namespace LEDSegments;
using SPEC = MatrixDisplaySpec;

std::unique_ptr<Display<SPEC>> display;

void setup() {
    Serial.begin(9600);
    delay(2000);
    
    display = std::make_unique<Display<SPEC>>();
}

void loop() {
    display->loop();
}
```

**Key Components:**
- **`MatrixDisplaySpec`**: Defines your LED layout, segments, and configuration
- **`Display<SPEC>`**: Main controller that handles effects, overlays, and transitions
- **Automatic Effects**: The display automatically cycles through different visual effects

### Creating Your Own Display Spec

1. **Copy** `examples/BasicUsage/spec/MatrixDisplaySpec.h` to your project
2. **Modify** the layout configuration for your specific LED setup:
   - Change `LED_PIN` for your data pin
   - Adjust `nbLeds()` for your strip length
   - Customize `segmentSize()` for your layout segments

## Troubleshooting

### Common Issues

**"LED-Segments.h: No such file or directory"**
- Make sure you installed the library correctly
- For PlatformIO: Check that `lib_deps` includes the GitHub URL

**"error: 'std::make_unique' was not declared"**
- Add `build_flags = -std=c++17` to your platformio.ini
- For Arduino IDE: Use Tools → Board → Board Manager and ensure C++17 support

**LEDs not lighting up**
- Check wiring connections
- Verify LED strip type matches `LED_TYPE` in your DisplaySpec
- Ensure adequate power supply for your LED count

**Compilation errors with FastLED**
- Make sure FastLED version is 3.10.0 or later
- Check that your board supports the LED strip type you're using

### Getting Help
- Check the complete working example in `examples/BasicUsage/`
- Review the `wiki/` directory for detailed documentation
- Open an issue on GitHub for specific problems

## Advanced Usage

For advanced customization, see:
- `wiki/DisplaySpec.md` - Creating custom display layouts
- `wiki/Effects.md` - Available effects and parameters  
- `wiki/Overlays.md` - Overlay effects for additional visual layers
- `wiki/Transitions.md` - Smooth transitions between effects
