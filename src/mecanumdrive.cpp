#include "mecanumdrive.h"

MecanumDrive::MecanumDrive() {}

MecanumDrive::MecanumDrive(MotorDriver *flm, MotorDriver *frm, MotorDriver *blm, MotorDriver *brm) {
  frontLeftMotor = flm;
  frontRightMotor = frm;
  backLeftMotor = blm;
  backRightMotor = brm;
}

//
// Drivetrain functions
//
void MecanumDrive::Enable() {
  frontLeftMotor->Enable();
  frontRightMotor->Enable();
  backLeftMotor->Enable();
  backRightMotor->Enable();
}
void MecanumDrive::Disable() {
  frontLeftMotor->Disable();
  frontRightMotor->Disable();
  backLeftMotor->Disable();
  backRightMotor->Disable();
}
/**
 * Handle Stick Inputs
 * 
 * Takes input from the directional sticks for the robot drive. Outputs
 * proper values and speeds to the motors.
 * 
 * @param x1 The number value from the channel mapping x1 input (1000 > x > 2000)
 * @param x2 The number value from the channel mapping x2 input (1000 > x > 2000)
 * @param y2 The number value from the channel mapping y2 input (1000 > x > 2000)
*/
void MecanumDrive::HandleStickInput(int x1, int x2, int y2) {
  float x1Normalized = norm(x1);
  float x2Normalized = norm(x2);
  float y2Normalized = norm(y2);

  int x1Speed = map(abs(x1Normalized) * 100.0f, 0, 100, 0, 255);
  int x2Speed = map(abs(x2Normalized) * 100.0f, 0, 100, 0, 255);
  int y2Speed = map(abs(y2Normalized) * 100.0f, 0, 100, 0, 255);
  int movementSpeed = max(max(x1Speed, x2Speed), y2Speed);
  float m1 = (x1Normalized+x2Normalized+y2Normalized)/3.0;
  float m2 = (x1Normalized-x2Normalized-y2Normalized)/3.0;
  float m3 = (x1Normalized-x2Normalized+y2Normalized)/3.0;
  float m4 = (x1Normalized+x2Normalized-y2Normalized)/3.0;

  frontLeftMotor->SetSignal(m1, movementSpeed);
  frontRightMotor->SetSignal(m2, movementSpeed);
  backLeftMotor->SetSignal(m3, movementSpeed);
  backRightMotor->SetSignal(m4, movementSpeed);
}
//
// API for autonomous driving
//
void MecanumDrive::Stop() {
  frontLeftMotor->Stop();
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->Stop();
}
void MecanumDrive::MoveForward(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnLeft(spd);
}
void MecanumDrive::MoveBackward(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnRight(spd);
}
void MecanumDrive::MoveLeft(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnRight(spd);
}
void MecanumDrive::MoveRight(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnLeft(spd);
}
void MecanumDrive::MoveLeftForward(uint8_t spd) {
  frontLeftMotor->Stop();
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->Stop();
}
void MecanumDrive::MoveRightForward(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->TurnLeft(spd);
}
void MecanumDrive::MoveLeftBackward(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->TurnRight(spd);
}
void MecanumDrive::MoveRightBackward(uint8_t spd) {
  frontLeftMotor->Stop();
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->Stop();
}
void MecanumDrive::RotateLeft(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnLeft(spd);
}
void MecanumDrive::RotateRight(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnRight(spd);
}
float MecanumDrive::norm(int in) {
  if (in < 1000) in = 1000;
  if (in > 2000) in = 2000;
  return (float(in-1500.0)) / 500.0; 
}