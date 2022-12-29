#include "drivers/timer.h"

HardwareTimer TimerOrchestrator::GetActiveTimer(int timer) {
    return activeTimers[timer];
}