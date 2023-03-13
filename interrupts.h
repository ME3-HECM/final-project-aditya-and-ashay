#ifndef _interrupts_H
#define _interrupts_H

#include <xc.h>

#define _XTAL_FREQ 64000000


//Value for the upper bound for the interrupt to be triggered
//Will find these through calibration
int Int_flag = 0;
int color_lowerbound;
int color_upperbound;

void interrupts_init(void);
void colorclick_interrupts_init(void);
void interrupts_clear(void);
void __interrupt(high_priority) HighISR();

#endif

