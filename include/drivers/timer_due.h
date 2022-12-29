#ifndef DRIVERS_TIMER_DUE_h
#define DRIVERS_TIMER_DUE_h
#include "drivers/timer.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class DueHardwareTimer: public HardwareTimer
{
    public:
        void Setup();
        void Begin(int (*func)(), unsigned int frequency);
};
#endif
