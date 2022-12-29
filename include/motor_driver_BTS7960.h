#ifndef BTS7960_h
#define BTS7960_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "motor_driver.h"

class BTS7960: public MotorDriver
{
  public:
    BTS7960(uint8_t EN, uint8_t L_PWM, uint8_t R_PWM) : BTS7960(EN, 0, L_PWM, R_PWM) {}
    BTS7960(uint8_t L_EN, uint8_t R_EN, uint8_t L_PWM, uint8_t R_PWM);
    BTS7960();
    void Enable();
    void Disable();

    void TurnLeft(uint8_t pwm);
    void TurnRight(uint8_t pwm);
    void Stop();

  private:
    uint8_t _L_EN;
    uint8_t _R_EN;
    uint8_t _L_PWM;
    uint8_t _R_PWM;
};
#endif
