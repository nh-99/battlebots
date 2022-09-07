#include "swervedrive.h"

SwerveDrive::SwerveDrive() {}

SwerveDrive::SwerveDrive(MotorDriver* frontLeftMotor, MotorDriver* frontRightMotor, MotorDriver* backLeftMotor, MotorDriver* backRightMotor) {
  frontLeftMotor = frontLeftMotor;
  frontRightMotor = frontRightMotor;
  backLeftMotor = backLeftMotor;
  backRightMotor = backRightMotor;
}

//
// Drivetrain functions
//
void SwerveDrive::Enable() {
  frontLeftMotor->Enable();
  frontRightMotor->Enable();
  backLeftMotor->Enable();
  backRightMotor->Enable();
}
void SwerveDrive::Disable() {
  frontLeftMotor->Disable();
  frontRightMotor->Disable();
  backLeftMotor->Disable();
  backRightMotor->Disable();
}
void SwerveDrive::Stop() {
  frontLeftMotor->Stop();
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->Stop();
}
void SwerveDrive::MoveForward(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnLeft(spd);
}
void SwerveDrive::MoveBackward(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnRight(spd);
}
void SwerveDrive::MoveLeft(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnRight(spd);
}
void SwerveDrive::MoveRight(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnLeft(spd);
}
void SwerveDrive::MoveLeftForward(uint8_t spd) {
  frontLeftMotor->Stop();
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->Stop();
}
void SwerveDrive::MoveRightForward(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->TurnLeft(spd);
}
void SwerveDrive::MoveLeftBackward(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->Stop();
  backLeftMotor->Stop();
  backRightMotor->TurnRight(spd);
}
void SwerveDrive::MoveRightBackward(uint8_t spd) {
  frontLeftMotor->Stop();
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->Stop();
}
void SwerveDrive::RotateLeft(uint8_t spd) {
  frontLeftMotor->TurnLeft(spd);
  frontRightMotor->TurnLeft(spd);
  backLeftMotor->TurnLeft(spd);
  backRightMotor->TurnLeft(spd);
}
void SwerveDrive::RotateRight(uint8_t spd) {
  frontLeftMotor->TurnRight(spd);
  frontRightMotor->TurnRight(spd);
  backLeftMotor->TurnRight(spd);
  backRightMotor->TurnRight(spd);
}
