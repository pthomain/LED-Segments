# BasicUsage Example - PlatformIO Project

This example demonstrates advanced usage of the LED-Segments library with complex display specifications, custom effects, and full C++17 features.

## Requirements

- **PlatformIO**
- **Seeeduino Xiao** or **ESP32** board
- **WS2812B LED strip** (256 LEDs configured for this example)
- **5V Power Supply** for LED strip

## Hardware Setup

- **LED Strip Data Pin** → Pin 9 (or change `LED_PIN` in MatrixDisplaySpec.h)
- **LED Strip VCC** → 5V power supply  
- **LED Strip GND** → Arduino GND + Power supply GND

## Getting Started

### 1. Install PlatformIO
- Install VSCode
- Install PlatformIO extension
- Restart VSCode

### 2. Open Project
- Open this `BasicUsage` folder in VSCode
- PlatformIO will automatically detect the project

### 3. Build and Upload
```bash
# Build the project
pio run

# Upload to board
pio run -t upload

# Monitor serial output
pio device monitor
```

### 4. Customize Your Setup

Edit `include/MatrixDisplaySpec.h` to modify:
- `LED_PIN` - Data pin for your LED strip
- `TOTAL_LEDS` - Number of LEDs in your strip
- `TOTAL_PIXELS` - Number of virtual pixels
- Effect configurations in `include/config/`

## Why PlatformIO?

This example uses C++17 features that Arduino IDE cannot compile:
- `std::make_unique` for modern memory management
- Complex template configurations
- Advanced library dependencies
- Sophisticated build system requirements

## Troubleshooting

**Build fails with C++17 errors:**
- Ensure your board supports C++17 (ESP32, Seeeduino Xiao)
- Check that `build_flags = -std=c++17` is in platformio.ini

**LEDs not working:**
- Verify wiring connections
- Check power supply capacity
- Confirm LED_TYPE matches your strip
- Ensure adequate voltage (5V for WS2812B)

**Upload fails:**
- Check board and port selection in PlatformIO
- Verify USB cable supports data transfer
- Try different USB port
