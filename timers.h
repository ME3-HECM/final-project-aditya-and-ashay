#ifndef _timers_H
#define _timers_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void);

/************************************
 * Function to reset timer
 * Used everytime a new card is reached so an accurate measurement of time can be made
************************************/
void timer_reset(void);

#endif

