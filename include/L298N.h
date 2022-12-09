#ifndef L298N_h
#define L298N_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "motor_driver.h"

class L298N: public MotorDriver
{
  public:
    L298N(uint8_t, uint8_t);
    L298N();
    void Enable();
    void Disable();

    void TurnLeft(uint8_t);
    void TurnRight(uint8_t);
    void SetSignal(float, int);
    void Stop();

  private:
    uint8_t _L_PWM;
    uint8_t _R_PWM;
};
#endif
