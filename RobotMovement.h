#include <DualMAX14870MotorShield.h>

/*
 * This example uses the DualMAX14870MotorShield library to drive each motor
 * with the Pololu Dual MAX14870 Motor Driver Shield for Arduino forward, then
 * backward. The yellow user LED is on when a motor is set to a positive speed
 * and off when a motor is set to a negative speed; it blinks rapidly if the
 * drivers report a fault.
 */

#define LED_PIN 13

DualMAX14870MotorShield motors;

boolean isFaulted = false;

int currentRobotSpeed[] = {0, 0};
const int MAX_SPEED = 400;

void stopIfFault()
{
  if (motors.getFault())
  {
    Serial.println("MOTOR CONTROL FAULT.");
    isFaulted = true;
    while (isFaulted)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
}

void driveStraight(boolean isForward) {
  int driveDir = isForward ? 1 : -1;
  currentRobotSpeed[0] = constrain(currentRobotSpeed[0] + driveDir, -MAX_SPEED, MAX_SPEED);
  currentRobotSpeed[1] = constrain(currentRobotSpeed[1] + driveDir, -MAX_SPEED, MAX_SPEED);
//  Serial.print("Spd: ");
//  Serial.println(currentRobotSpeed[0]);
//  Serial.println(currentRobotSpeed[1]);
  motors.setSpeeds(currentRobotSpeed[0],currentRobotSpeed[1]);
  delay(4);
  stopIfFault();
}

void turnInPlace(boolean isTurnLeft) {
  int turnVal = isTurnLeft ? -1 : 1;
  currentRobotSpeed[0] = constrain(currentRobotSpeed[0] + turnVal, -MAX_SPEED, MAX_SPEED);
  currentRobotSpeed[1] = constrain(currentRobotSpeed[1] - turnVal, -MAX_SPEED, MAX_SPEED);
  motors.setSpeeds(currentRobotSpeed[0],currentRobotSpeed[1]);
  delay(2);
  stopIfFault();  
}

void turnAndRotate(boolean isForward, boolean isTurnLeft) {
  // FL 200 400  
  // FR 400 200
  // BL -200 -400
  // BR -400 -200
  int driveDir = isForward ? 1 : -1;
  if (isTurnLeft) {
    currentRobotSpeed[0] = constrain(currentRobotSpeed[0] + (driveDir * 1), -MAX_SPEED / 2, MAX_SPEED / 2);
    currentRobotSpeed[1] = constrain(currentRobotSpeed[1] + (driveDir * 2), -MAX_SPEED, MAX_SPEED);
  } else {
    currentRobotSpeed[0] = constrain(currentRobotSpeed[0] + (driveDir * 2), -MAX_SPEED, MAX_SPEED);
    currentRobotSpeed[1] = constrain(currentRobotSpeed[1] + (driveDir * 1), -MAX_SPEED / 2, MAX_SPEED / 2);
  }
  motors.setSpeeds(currentRobotSpeed[0],currentRobotSpeed[1]);
  delay(2);
  stopIfFault();  
}


void stopRobot() {

  for (int motorIdx = 0; motorIdx < 2; motorIdx++) {
    if (currentRobotSpeed[motorIdx] > 0) {
      currentRobotSpeed[motorIdx] = constrain(currentRobotSpeed[motorIdx] -= 1, 0, MAX_SPEED);  
    } else {
      currentRobotSpeed[motorIdx] = constrain(currentRobotSpeed[motorIdx] += 1, -MAX_SPEED, 0);      
    }
  }
  motors.setSpeeds(currentRobotSpeed[0],currentRobotSpeed[1]);
  delay(2);
  stopIfFault();

//    Serial.print("Spd: ");
//    Serial.println(currentRobotSpeed);
}

void emergencyStop() {
  currentRobotSpeed[0] = 0;
  currentRobotSpeed[1] = 0;
  motors.setSpeeds(0, 0);
  stopIfFault();
}
