#ifndef _color_H
#define _color_H

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

typedef struct colors {
    unsigned int R; //red channel reading
    unsigned int B; //blue channel reading
    unsigned int G; //green channel reading
    unsigned int C; // clear channel reading
    float R_norm; // Red/Clear
    float B_norm; // Blue/Clear
    float G_norm; // Green/Clear
} colors;

struct colors color;

int color_upperbound = 2500; // if clear channel is above this value a card will have been reached (obtained through calibration)

unsigned char card_count = 0; //index for card_memory array
char card_memory[40]; // empty array to hold card numbers

unsigned char timer_index = 0; //index for timer_array
char timer_memory[40]; //empty array to hold timings


/*****************************************
* Function used to initialise colour click
*****************************************/
void color_click_init(void);

/*****************************************
* Function used to write to the colour click
*****************************************/
void color_writetoaddr(char address, char value);

/*****************************************
* Function used to read the red light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Red(void);

/*****************************************
* Function used to read the blue light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Blue(void);

/*****************************************
* Function used to read the green light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Green(void);

/*****************************************
* Function used to read the clear light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Clear(void);

/*****************************************
* Function used to perform colour reads and assign them to struct variables
* normalised values are also added to the colour struct 
*****************************************/
void READcolor(colors *c);

/*****************************************
* Function used to read colours and create the desired response of the buggy
* Function also assigns colour read and timer values to an array to  be used in the return home function
*****************************************/
void buggy_color_response(DC_motor *mL,DC_motor *mR, colors *c);

/*****************************************
* Function used to bring buggy back to start position
* Iterates through timer array and card array from back to front 
*****************************************/
void return_home(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to create a custom delay for the timer
* Our timer overflows every 131 ms therfore this function takes a timer overflow input and creates the desired delay time
*****************************************/
void delay_ms_func(unsigned int time);


#endif





