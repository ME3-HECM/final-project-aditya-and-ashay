#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void interrupts_init(void)
{
    PIE0bits.TMR0IE = 1; //Enable Timer interrupt source
    PIR0bits.TMR0IF = 0; //Clear interrupt flag
    IPR0bits.TMR0IP = 0; // Set timer interrupt to low priority
    
    INTCONbits.IPEN = 1;    // Enable priority levels on interrupts
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts last, once all other interrupt configuration is done (when this is off, all interrupts are deactivated)
}

/************************************
 * Low priorty interrupt flags everytime the timer overflows (131 ms)
************************************/
void __interrupt(low_priority) LowISR() 
{
    if (PIR0bits.TMR0IF) { //Check interrupt source
        timer_val += 1; //adds 1 to timer val (each 1 is worth 131 ms)
        PIR0bits.TMR0IF = 0; // Clear interrupt flag
    }
  
}





