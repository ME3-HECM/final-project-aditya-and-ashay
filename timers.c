#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1110; // 1:16384 Prescaler so that each bit = 1.024 ms
    T0CON0bits.T016BIT=1;	//16bit mode	           

    TMR0H=0;            
    TMR0L=0;   
    
    T0CON0bits.T0EN=1;	//start the timer
}

void timer_reset(void) {
    TMR0H = 0;
    TMR0L = 0;
}
/************************************
 * Function to return the full 16bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
************************************/
unsigned int get_timer_val(void)
{
    unsigned int timer_val = TMR0L | (TMR0H << 8);
    
    return timer_val;
}

