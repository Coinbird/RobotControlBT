#include "RobotState.h"
#include "RobotMovement.h"

long maxTimeoutMS = 150; // not const in case we want to increase this via command

RobotState state;
unsigned long prevCmdMillis = 0;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(38400);
  Serial.println("Robot motion control initialized.");
  
  // TODO: increase Serial1 comm rate for speed via AT command to HC-05
  Serial1.begin(9600);  // HC-05 default speed
  
  state = RobotState::Fault;
  prevCmdMillis = millis();

  motors.flipM1(true);
//  motors.flipM2(true);
}

void gotCmd() {
    prevCmdMillis = millis();
}

void changeState(RobotState newState) {
  // Change state
   if(state != newState) {
    Serial.print("Entered State =");
    Serial.println(newState); // TODO Translate
    handleEnterState(newState);
    state = newState;
  }
}

void handleEnterState(RobotState newState) {
  switch(newState) {
    case RobotState::Forward:
      Serial.println("F");
      break;
    case RobotState::Back:
      Serial.println("B");
      break;
    case RobotState::Stopped:
      Serial.println("Slow/STOP");
      break;
    case RobotState::Fault:
    default:
      Serial.println("E-STOP");
      emergencyStop();
      break;    
  }
}

void handleStateLoop() {
  // In a state, perform updates
  switch(state) {
    case RobotState::Forward:
      driveForward();
      break;
    case RobotState::Back:
      driveBack();
      break;
    case RobotState::Stopped:
      stopRobot();
      break;
  }
}

void loop()
{
  // Single-char Commands, sent at high rate from the BT application:
  // S: stop (default state)
  // R, L, F, B: right, left, forward, back
  // G, I, H, J: FL, FR, BL, BR
  // Single Commands:
  // D: bluetooth disconnecting now
  // TODO: others

  motors.enableDrivers();
  
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  Serial1.flush();
  if (Serial1.available()) {
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
      default:
          Serial.println(readByte);
          break;
    }
  }

  unsigned long curMillis = millis();
  if (curMillis - prevCmdMillis > maxTimeoutMS && state != RobotState::Fault) {
    Serial.print("Entered FAULT State, command not received in ");
    Serial.print(maxTimeoutMS);
    Serial.print(" ms.");
    changeState(RobotState::Fault);
  } 
  handleStateLoop();
}
