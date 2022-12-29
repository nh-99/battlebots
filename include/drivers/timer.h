#ifndef DRIVERS_TIMER_h
#define DRIVERS_TIMER_h
#include "global.h"

class HardwareTimer
{
    private:
        bool stopped = false;
    public:
        int (*functionToRun)();
        virtual void Setup() {};
        virtual void Begin(int (*func)(), unsigned int frequency) {};
};

class TimerOrchestrator
{
    private:
        static const unsigned int maxTimers = MAX_TIMERS;
        unsigned int timerCount;
        HardwareTimer activeTimers[maxTimers];
        HardwareTimer suspendedTimers[maxTimers * 4];
    public:
        void AddTimer(HardwareTimer);
        void ResumeTimer(HardwareTimer);
        void SuspendTimer(HardwareTimer);
        HardwareTimer GetActiveTimer(int timer);
};
#endif
