#include "RobotState.h"
#include "RobotVision.h"

long maxTimeoutMS = 500; // not const in case we want to increase this via command

// Timing
static unsigned long prevCmdMillis = 0;
static unsigned long curMillis = 0;

void setup()
{

  initRobotLEDs();
  allLEDs(255,0,0);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(38400);
  
  // TODO: increase Serial1 comm rate for speed via AT command to HC-05
  Serial1.begin(9600);  // HC-05 default speed

  delay(500); // Give motor driver a sec to get started
  prevCmdMillis = millis();

  initRobotState();
  initRobotVision();
  Serial.println("Robot initialized.");

  if (stopIfFault()) {
    state = RobotState::Error;
  }
}

void gotCmd() {
    prevCmdMillis = millis();
}

void loop()
{  
  curMillis = millis();

  motors.enableDrivers();
  loopPixelsBlink(curMillis, state);
  performVision();
    
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  Serial1.flush();
  if (Serial1.available()) {
    // Single-char Commands, sent at high rate from the BT application:
    // S: stop (default state)
    // R, L, F, B: right, left, forward, back
    // G, I, H, J: FL, FR, BL, BR
    // Single Commands:
    // D: bluetooth disconnecting now
    // W: headlights/ vision mode ON
    // w: headlights/ vision mode OFF
    // TODO: others

    gotCmd();
    char readByte = Serial1.read();
    if (!isVisionModeEnabled) {
      switch(readByte) {
        case 'S':
            changeState(RobotState::Stopped);
            break;
        case 'R':
            changeState(RobotState::TurnRight);
            break;
        case 'L':
            changeState(RobotState::TurnLeft);
            break;
        case 'F':
            changeState(RobotState::Forward);
            break;
        case 'B':
            changeState(RobotState::Back);
            break;
        case 'G':
            changeState(RobotState::ForwardLeft);
            break;
        case 'I':
            changeState(RobotState::ForwardRight);
            break;
        case 'H':
            changeState(RobotState::BackLeft);
            break;
        case 'J':
            changeState(RobotState::BackRight);
            break;
        case 'D':
            Serial.println("Manual Disconnect received.");
            changeState(RobotState::Fault);
            break;
        case 'W':
            Serial.println("VISION MODE.");
            isVisionModeEnabled = true;
            break;
        default:
            // Unhandled command - output for future addition
            Serial.println(readByte);
            break;
      }
     } else {
        switch(curVizCmd) {
          case VisionCommand::VisionStop: 
             changeState(RobotState::Stopped);
             break;
          case VisionCommand::VisionForward: 
             changeState(RobotState::Forward);
             break;
          case VisionCommand::VisionForwardLeft:
            changeState(RobotState::ForwardLeft);
            break;
         case VisionCommand::VisionForwardRight:
            changeState(RobotState::ForwardRight);
            break;
         case VisionCommand::VisionBack:
            changeState(RobotState::Back);
            break;
        }
        // Vision mode: still handle important stuff        
        switch(readByte) {
          case 'D':
              Serial.println("Manual Disconnect received.");
              isVisionModeEnabled = false;
              changeState(RobotState::Fault);
              break;
          case 'w':
              Serial.println("Vision off.");
              isVisionModeEnabled = false;
              break;
          default:
              // Ignore command
//              Serial.println(readByte);
              break;        
        }
        
    }
  }

  if (curMillis - prevCmdMillis > maxTimeoutMS && state != RobotState::Fault && state != RobotState::Error) {
    Serial.print("Entered Fault/Stop State, command not received in ");
    Serial.print(maxTimeoutMS);
    Serial.print(" ms.");
    changeState(RobotState::Fault);
  } 
  handleStateLoop();
}
