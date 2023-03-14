#ifndef _color_H
#define _color_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct colors {
    unsigned int R;
    unsigned int B;
    unsigned int G;
    unsigned int C;
    float R_norm;
    float B_norm;
    float G_norm;
} colors;

struct colors color;

int color_lowerbound = 0;
int color_upperbound = 2500;

unsigned char card_count = 0;
char card_memory[40];

unsigned char timer_index = 0;
char timer_memory[40];


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
void READcolor(colors *c);
void buggy_color_response(DC_motor *mL,DC_motor *mR, colors *c);

void colourcards_normaliseRGBC(colors *c);

void return_home(DC_motor *mL, DC_motor *mR);
void delay_ms_func(unsigned int time);
//void clear_RBG(colors *c);
//void calibrate_upperbound(colors *c);

#endif





