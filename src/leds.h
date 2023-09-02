#include <FastLED.h>
#include <effects.h>

 void initLeds()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    set_max_power_in_volts_and_milliamps(5, 500);
    FastLED.clear();
}

