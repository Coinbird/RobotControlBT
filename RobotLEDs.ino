#include <Adafruit_NeoPixel.h>
#include "RobotState.h"

#define PIN 50
#define NUMPIXELS 2

#define LEFT_PIXEL 0
#define RIGHT_PIXEL 1

static unsigned long lastBlink = 0;
static unsigned long blinkTime = 50;

// Define blink sequences
#define SEQ_LEN 8
static const boolean leftSeq[] = {false, true, false, true, false, false, false, false}; // blink pattern
static const boolean rightSeq[] = {false, false, false, false, false, true, false, true}; // opposite blink pattern
static byte seqIdx = 0;

#define YELLOW
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

void loopPixelsBlink(unsigned long& curMillis, RobotState state) {
  switch(state) {
    case RobotState::Error:
      allLEDs(150, 0, 0);
      break;
    case RobotState::Fault:
      allLEDs(0, 150, 0); 
      break;
    default:
      if (curMillis - lastBlink > blinkTime){
        lastBlink = curMillis;
        uint8_t lColor = leftSeq[seqIdx] ? 255 : 0;
        uint8_t rColor = rightSeq[seqIdx++] ? 255 : 0;
        pixels.setPixelColor(LEFT_PIXEL, pixels.Color(lColor, lColor, 0));
        pixels.setPixelColor(RIGHT_PIXEL, pixels.Color(rColor, rColor, 0));
        seqIdx %= SEQ_LEN; // loop sequence
        
        pixels.show();   // Send the updated pixel colors to the hardware.
      } 
      break;
  }
}
