#include <SPI.h>  
#include <Pixy.h>


// Vision system state machine, commands issued based on Pixy color code locations
enum VisionCommand {
  VisionStop, // 0
  VisionForward, // 1
  VisionBack, // 2
  VisionForwardLeft, // 3
  VisionForwardRight  // 4
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



const uint16_t MAX_BACKUP_WIDTH = 150; // if exceeded, back up
const uint16_t MIN_STOP_WIDTH = 45; // if less than, drive forward. Increase to keep closer.
const uint16_t DEAD_BAND = 20; // midpoint dead band
const uint16_t MIDDLE_SCREEN = 320 / 2; // middle of screen

VisionCommand recommendCommand(uint16_t width, uint16_t xPos) {
   if (width > MAX_BACKUP_WIDTH){
     return VisionCommand::VisionBack;
   } else if (width > MIN_STOP_WIDTH) {
     return VisionCommand::VisionStop;
   } else if (xPos < MIDDLE_SCREEN - DEAD_BAND) {
     return VisionCommand::VisionForwardLeft;
   } else if (xPos > MIDDLE_SCREEN + DEAD_BAND) {
     return VisionCommand::VisionForwardRight;
   } else {
     return VisionCommand::VisionForward;
   }
}


/**
 * Perform object detection and 
 */

const uint16_t ALLOWED_CC_1 = 10; // 12 octal
const uint16_t ALLOWED_CC_2 = 81; // 121 octal

/**
 * Performs vision and updates curVizCmd
 */
boolean performVision() {
  int maxSizeIdx = -1;
  uint16_t maxHeight = 0;
//  curVizCmd = VisionCommand::VisionStop;
  if (isVisionModeEnabled) {
    blocks = pixy.getBlocks();
    if (blocks) {
        for (int j=0; j<blocks; j++)
        {   // Try to guess the best block
            if ((pixy.blocks[j].signature == ALLOWED_CC_1 || 
                pixy.blocks[j].signature == ALLOWED_CC_2) &&
                pixy.blocks[j].height > maxHeight) {
              maxHeight = pixy.blocks[j].height;
              maxSizeIdx = j;
            }
        }
        if(maxSizeIdx >= 0) {
//          Serial.print("Thiccest: ");
//          Serial.print(pixy.blocks[maxSizeIdx].signature, OCT);
//          Serial.println();
//          pixy.blocks[maxSizeIdx].print();
          curVizCmd = recommendCommand(pixy.blocks[maxSizeIdx].width, pixy.blocks[maxSizeIdx].x);
//          Serial.print("COMMAND: ");
//          Serial.println(curVizCmd); 
        } else {
          Serial.println("?? Inconclusive vision!");  
        }
    }
  }
}
