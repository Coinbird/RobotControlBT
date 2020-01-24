#include <SPI.h>  
#include <Pixy.h>


// Vision system state machine, commands issued based on Pixy color code locations
enum VisionCommand {
  VisionStop,
  VisionForward,
  VisionBack,
  VisionForwardLeft,
  VisionForwardRight
};


// This is the main Pixy object 
Pixy pixy;

// Pixy Vision Related Globals
// unsigned int blockCounter=0;
uint16_t blocks;
char buf[32]; 

boolean isVisionModeEnabled = false;

VisionCommand curVizCmd = VisionCommand::VisionStop;

void initRobotVision() {
    pixy.init();
}

/**
 * Perform object detection and 
 */

const uint16_t ALLOWED_CC_1 = 10; // 12 octal
const uint16_t ALLOWED_CC_2 = 81; // 121 octal

/**
 * Performs vision and updates curVizCmd
 */
void performVision() {
  int maxSizeIdx = -1;
  uint16_t maxHeight = 0;
  curVizCmd = VisionCommand::VisionStop;
  if (isVisionModeEnabled) {
    blocks = pixy.getBlocks();
    if (blocks) {
        for (int j=0; j<blocks; j++)
        {
            if (pixy.blocks[j].height > maxHeight) {
              maxHeight = pixy.blocks[j].height;
              maxSizeIdx = j;
            }
        }
        Serial.print("Thiccest: ");
        Serial.print(pixy.blocks[maxSizeIdx].signature, OCT);
        Serial.println();
        pixy.blocks[maxSizeIdx].print();
    }
  }
}
