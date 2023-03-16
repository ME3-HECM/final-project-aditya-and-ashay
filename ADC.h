#ifndef _ADC_H
#define _ADC_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/************************************
* Function used to initialise ADC module 
* to sample on pin F6 
************************************/
void ADC_init(void);

/************************************
* Function used to return 8 bit number related to health of battery 
************************************/
unsigned int ADC_getval(void);

#endif
