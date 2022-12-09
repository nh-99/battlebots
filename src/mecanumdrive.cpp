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
float norm(int in) {
  if (in < 1000) in = 1000;
  if (in > 2000) in = 2000;
  return (float(in-1500.0)) / 500.0; 
}
void MecanumDrive::HandleStickInput(int forward, int strafe, int turn) {
  float forwardNormalized = norm(forward);
  float strafeNormalized = norm(strafe);
  float turnNormalized = norm(turn);

  int forwardSpeed = map(abs(forwardNormalized) * 100.0f, 0, 100, 0, 255);
  int strafeSpeed = map(abs(strafeNormalized) * 100.0f, 0, 100, 0, 255);
  int turnSpeed = map(abs(turnNormalized) * 100.0f, 0, 100, 0, 255);
  int movementSpeed = max(max(forwardSpeed, strafeSpeed), turnSpeed);
  float m1 = (forwardNormalized+strafeNormalized+turnNormalized)/3.0;
  float m2 = (forwardNormalized-strafeNormalized-turnNormalized)/3.0;
  float m3 = (forwardNormalized-strafeNormalized+turnNormalized)/3.0;
  float m4 = (forwardNormalized+strafeNormalized-turnNormalized)/3.0;

  frontLeftMotor->SetSignal(m1, movementSpeed);
  frontRightMotor->SetSignal(m2, movementSpeed);
  backLeftMotor->SetSignal(m3, movementSpeed);
  backRightMotor->SetSignal(m4, movementSpeed);
}


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
