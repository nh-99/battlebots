#include "drivers/timer_due.h"

volatile bool l;

void startTimer(Tc *tc, unsigned int channel, IRQn_Type irq, unsigned int frequency) {
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk((uint32_t) irq);
    TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    uint32_t rc = VARIANT_MCK / 128 / frequency; //128 because we selected TIMER_CLOCK4 above
    TC_SetRA(tc, channel, rc / 2); //50% high, 50% low
    TC_SetRC(tc, channel, rc);
    TC_Start(tc, channel);
    tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
    tc->TC_CHANNEL[channel].TC_IDR =~ TC_IER_CPCS;
    NVIC_EnableIRQ(irq);
}

void DueHardwareTimer::Begin(int (*func)(), unsigned int frequency) {
    functionToRun = func;

    int timerCount = 0;
    switch (timerCount) {
        case 0:
            startTimer(TC1, 0, TC3_IRQn, frequency);
            break;
        case 1:
            startTimer(TC1, 1, TC4_IRQn, frequency);
            break;
        case 2:
            startTimer(TC1, 2, TC5_IRQn, frequency);
            break;
        default:
            break;
            
    }

    //  ISR/IRQ   TC    Channel   Due pins
    //  TC0       TC0   0         2, 13
    //  TC1       TC0   1         60, 61
    //  TC2       TC0   2         58
    //  TC3       TC1   0         none
    //  TC4       TC1   1         none <- this line in the example above
    //  TC5       TC1   2         none
    //  TC6       TC2   0         4, 5
    //  TC7       TC2   1         3, 10
    //  TC8       TC2   2         11, 12
}

// void Setup() {
//     startTimer(TC1, 1, TC4_IRQn, 4); // TC1 channel 1, the IRQ for that channel and the desired frequency
// }