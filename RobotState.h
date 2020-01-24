#include "RobotMovement.h"

// High level state machine for driving robot
enum RobotState {
  Fault,
  Stopped,
  Forward,
  Back,
  TurnRight,
  TurnLeft,
  ForwardRight,
  ForwardLeft,
  BackRight,
  BackLeft
};

// Globals
RobotState state;

void initRobotState() {
  state = RobotState::Fault;
  motors.flipM1(true);
//  motors.flipM2(true);
}


/**
 * Called once as robot enters a new state
 */
void handleEnterState(RobotState newState) {
  switch(newState) {
    case RobotState::Forward:
    case RobotState::Back:
    case RobotState::TurnLeft:
    case RobotState::TurnRight:
    case RobotState::ForwardLeft:
    case RobotState::ForwardRight:
    case RobotState::BackLeft:
    case RobotState::BackRight:
      // valid implemented motions, continue
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

void showState(RobotState robotState) {
  switch(robotState) {
    case RobotState::Fault:
      Serial.println("Fault");
      break;
    case RobotState::Stopped:
      Serial.println("Stopped");
      break;
    case RobotState::Forward:
      Serial.println("Forward");
      break;
    case RobotState::Back:
      Serial.println("Back");
      break;
    case RobotState::TurnRight:
      Serial.println("TurnRight");
      break;
    case RobotState::TurnLeft:
      Serial.println("TurnLeft");
      break;
    case RobotState::ForwardRight:
      Serial.println("ForwardRight");
      break;
    case RobotState::ForwardLeft:
      Serial.println("ForwardLeft");
      break;
    case RobotState::BackRight:
      Serial.println("BackRight");
      break;
    case RobotState::BackLeft:
      Serial.println("BackLeft");
      break;
    default:
      Serial.println(robotState);
      break;
  }
}
void changeState(RobotState newState) {
  // Change state
   if(state != newState) {    
    Serial.print("Entered State =");
    showState(newState);
    handleEnterState(newState);
    state = newState;
  }
}

void handleStateLoop() {
  // In a state, perform updates
  switch(state) {
    case RobotState::Forward:
      driveStraight(true);
      break;
    case RobotState::Back:
      driveStraight(false);
      break;
    case RobotState::TurnLeft:
      turnInPlace(true);
      break;
    case RobotState::TurnRight:
      turnInPlace(false);
      break;
    case RobotState::ForwardLeft:
      turnAndRotate(true, true);
      break;
    case RobotState::ForwardRight:
      turnAndRotate(true, false);
      break;
    case RobotState::BackLeft:
      turnAndRotate(false, true);
      break;
    case RobotState::BackRight:
      turnAndRotate(false, false);
      break;
    case RobotState::Stopped:
      stopRobot();
      break;
  }
}
