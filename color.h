#ifndef _color_H
#define _color_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct colors_normalised {
    unsigned int R;
    unsigned int B;
    unsigned int G;
    unsigned int C;
    unsigned float R_norm;
    unsigned float B_norm;
    unsigned float G_norm;
} colors;

/********************************************//**
 *  Function to initialise the colour click module using I2C
 ***********************************************/
void color_click_init(void);

/********************************************//**
 *  Function to write to the colour click module
 *  address is the register within the colour click to write to
 *	value is the value that will be written to that address
 ***********************************************/
void color_writetoaddr(char address, char value);

/********************************************//**
 *  Function to read the red channel
 *	Returns a 16 bit ADC value representing colour intensity
 ***********************************************/
unsigned int color_read_Red(void);
unsigned int color_read_Blue(void);
unsigned int color_read_Green(void);
unsigned int color_read_Clear(void);
/***********************************
*Function to read the colour values and assign them to a struct
************************************/
void READcolor(void);


#endif
