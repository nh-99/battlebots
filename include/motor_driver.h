#ifndef MOTOR_DRIVER_h
#define MOTOR_DRIVER_h

class MotorDriver
{
  public:
    virtual void Enable();
    virtual void Disable();

    virtual void TurnLeft(uint8_t);
    virtual void TurnRight(uint8_t);
    virtual void SetSignal(float, int);
    virtual void Stop();
};
#endif
