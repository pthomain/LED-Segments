#define LED_PIN 9
#include <FastLED.h>

#define LED_TYPE WS2812B
#define LED_BRIGHTNESS 6
#define COLOR_ORDER GRB
#define NUM_LEDS 256
#define NUM_LETTERS 16

CRGB rawleds[NUM_LEDS];
CRGBSet leds(rawleds, NUM_LEDS);


