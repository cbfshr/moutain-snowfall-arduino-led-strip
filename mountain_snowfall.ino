#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <FastLED.h>

typedef int bool;
#define true 1
#define false 0
#define b_to_s(x) (x ? "true" : "false")

#define LED_PIN 3
#define NUM_LEDS 50
#define MAX_BRIGHTNESS 255
#define DEFAULT_BRIGHTNESS 64
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

struct snowflake {
  int brightness;
  bool active;
  bool falling;
} snowflakes[NUM_LEDS];

int snowfallPercentage = 50;
int numActiveSnowflakes = 0;
int i = 0;

void loop() {
  // If snowflakes have stopped falling, we still need to meet the snowfall percentage
  while (numActiveSnowflakes < NUM_LEDS * (snowfallPercentage / 100.0)) {
    int newSnowflakeIndex = rand() % NUM_LEDS;
    if (snowflakes[newSnowflakeIndex].active == false) {
      snowflakes[newSnowflakeIndex].active = true;
      numActiveSnowflakes++;
    }
  }
  
  // Iterate over all the snowflakes and change the values accordingly
  for (i = 0; i < NUM_LEDS; i++) {
    if (snowflakes[i].active) {
      if (snowflakes[i].falling == true) {
        if (snowflakes[i].brightness == 0) {
          snowflakes[i].falling = false;
          snowflakes[i].brightness = 0;
          snowflakes[i].active = false;
          numActiveSnowflakes--;
        } else {
          snowflakes[i].brightness--;
        }
      } else {
        if (snowflakes[i].brightness == MAX_BRIGHTNESS - 1) {
          snowflakes[i].falling = true;
        }
        snowflakes[i].brightness++;
      }
    }

    leds[i] = CRGB(snowflakes[i].brightness, snowflakes[i].brightness, snowflakes[i].brightness);
  }
  
  FastLED.show();
  delay(5);
}

void setup() {
  delay(100);

  // Initialize LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  srand(time(0));

  // Set all LEDs to off (black)
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(100);

  // Initialize Snowflakes - Set on/off, brightness, falling up/down
  for (i = 0; i < NUM_LEDS; i++) {
    snowflakes[i].active = rand() % 100 < snowfallPercentage;
    if (snowflakes[i].active) {
      snowflakes[i].brightness = rand() % MAX_BRIGHTNESS;
      snowflakes[i].falling = rand() % 2 == 0;
      numActiveSnowflakes++;
    } else {
      snowflakes[i].brightness = 0;
      snowflakes[i].falling = false;
    }
  }
}
