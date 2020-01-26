#include <Adafruit_NeoPixel.h>
#include "RobotState.h"

#define PIN 50
#define NUMPIXELS 2

#define LEFT_PIXEL 0
#define RIGHT_PIXEL 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void initRobotLEDs() {
    pixels.begin();
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();   // Send the updated pixel colors to the hardware.
}

void allLEDs(uint16_t r, uint16_t g, uint16_t b) {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
}

void loopPixelsBlink(RobotState state) {
  switch(state) {
    case RobotState::Error:
      allLEDs(150, 0, 0);
      break;
    case RobotState::Fault:
      allLEDs(0, 0, 150);
      break;
    default:
      allLEDs(0, 150, 0);
      break;
  }
}
