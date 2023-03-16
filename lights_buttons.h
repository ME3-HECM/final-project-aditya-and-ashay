
#ifndef LIGHTS_BUTTONS_H
#define	LIGHTS_BUTTONS_H

#include <xc.h>

#define _XTAL_FREQ 64000000

#define HEADLAMPS LATHbits.LATH1 // Toggles the front white LEDs and rear red LEDs (at a reduced brightness)
#define MAINLIGHT LATDbits.LATD3  // Toggles the front white LEDs (at full brightness)
#define BRAKELIGHT LATDbits.LATD4     // Toggles the rear red LEDs (at full brightness)
#define LEFTINDICATOR LATFbits.LATF0  // Toggles the left signal (but no hardware based brightness control)
#define RIGHTINDICATOR LATHbits.LATH0 // Toggles the right signal (but no hardware based brightness control)
#define LEDD7 LATDbits.LATD7
#define LEDH3 LATHbits.LATH3

/************************************************************
* Function used to initialise all the pins and buttons needed
************************************************************/
void ports_init(void);

/************************************************************
* Function used to initialise all the pins for the lights on the buggy
************************************************************/
void buggyLEDs_init(void);

#endif	

