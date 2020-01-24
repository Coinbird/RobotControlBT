#include "RobotState.h"
#include "RobotVision.h"

long maxTimeoutMS = 500; // not const in case we want to increase this via command

// Timing
static unsigned long prevCmdMillis = 0;
static unsigned long curMillis = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(38400);
  Serial.println("Robot motion control initialized.");
  
  // TODO: increase Serial1 comm rate for speed via AT command to HC-05
  Serial1.begin(9600);  // HC-05 default speed
  
  prevCmdMillis = millis();

  initRobotState();
  initRobotVision();
}

void gotCmd() {
    prevCmdMillis = millis();
}

void loop()
{  

  motors.enableDrivers();
  
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
      case 'w':
          Serial.println("Vision off.");
          isVisionModeEnabled = false;
          break;
      default:
          Serial.println(readByte);
          break;
    }
  }

  curMillis = millis();
  if (curMillis - prevCmdMillis > maxTimeoutMS && state != RobotState::Fault) {
    Serial.print("Entered FAULT State, command not received in ");
    Serial.print(maxTimeoutMS);
    Serial.print(" ms.");
    changeState(RobotState::Fault);
  } 
  handleStateLoop();
}
