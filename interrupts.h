#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000


//Global varibale to count forward time movements of buggy
int timer_val = 0;

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void interrupts_init(void);

/************************************
 * Low priorty interrupt flags everytime the timer overflows (131 ms)
************************************/
void __interrupt(low_priority) LowISR();

#endif