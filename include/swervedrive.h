#ifndef SWERVEDRIVE_H
#define SWERVEDRIVE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "motor_driver.h"

class SwerveDrive
{
  public:
    SwerveDrive(MotorDriver*, MotorDriver*, MotorDriver*, MotorDriver*);
    SwerveDrive();
    void Enable();
    void Disable();

    void Stop();
    void MoveForward(uint8_t);
    void MoveBackward(uint8_t);
    void MoveLeft(uint8_t);
    void MoveRight(uint8_t);
    void MoveLeftForward(uint8_t);
    void MoveRightForward(uint8_t);
    void MoveLeftBackward(uint8_t);
    void MoveRightBackward(uint8_t);
    void RotateLeft(uint8_t);
    void RotateRight(uint8_t);

  private:
    MotorDriver * frontLeftMotor;
    MotorDriver * frontRightMotor;
    MotorDriver * backLeftMotor;
    MotorDriver * backRightMotor;
};
#endif
