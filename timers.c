#include <xc.h>
#include "timers.h"
#include "interrupts.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; 
    T0CON1bits.T0CKPS=0b0101; // 1:32 prescalar rate bit
    T0CON0bits.T016BIT=1;	//16 bit mode	
	
    
    TMR0H=0;          //write High reg first
    TMR0L=0b00100011; // Initliasied as 35 to ensure timer overflows at exactly 131 ms
    T0CON0bits.T0EN=1;	//start the timer
}

/************************************
 * Function to reset timer
 * Used everytime a new card is reached so an accurate measurement of time can be made
************************************/
void timer_reset(void) {
    timer_val = 0;      // set timer value back to 0
    TMR0H=0;            
    TMR0L=0b00100011;   //set timer back to initial value of 35
}
