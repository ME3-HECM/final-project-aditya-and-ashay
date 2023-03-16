#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#define _XTAL_FREQ 64000000

int left_timer = 98; // Defines delay time for left_45 turns
int right_timer = 96; // Defines delay time for right_45 turns
//int square_timer = 900; // Defines delay time for 1 square reverse 

typedef struct DC_motor { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    char brakemode;		// short or fast decay (brake or coast)
    unsigned int PWMperiod; //base period of PWM cycle
    unsigned char *posDutyHighByte; //PWM duty address for motor +ve side
    unsigned char *negDutyHighByte; //PWM duty address for motor -ve side
} DC_motor;

struct DC_motor motorL, motorR; 		//declare two DC_motor structures

//function prototypes
/************************************************************
* Function used to initialise T2 and CCP for DC motor control
************************************************************/
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM

/**********************************************************************
* Function to set CCP PWM output from the values in the motor structure
**********************************************************************/
void setMotorPWM(DC_motor *m);

/*********************************************
* Function to initialise motor specifications 
*********************************************/
void motor_init(DC_motor *mL,DC_motor *mR);

/************************************
* Function used to move buggy forward
************************************/
void forward(DC_motor *mL, DC_motor *mR);

/*****************************************************
* Function used to move buggy in the reverse direction
*****************************************************/
void reverse(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to stop buggy gradually
*****************************************/
void stop(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to make buggy turn left 45 degrees
* count input defines how many 45 degree turns the function will perform 
* left_45(mL,mR,2,left_timer) - This will do two 45 degree turns therefore make a 90 degree turn
*****************************************/
void left_45(DC_motor *mL, DC_motor *mR, int count, int left_timer);

/*****************************************
* Function used to make buggy turn right 45 degrees
* count input defines how many 45 degree turns the function will perform  
* right_45(mL, mR, 2, right_timer) - This will do two 45 degree turns therefore make a 90 degree turn
*****************************************/
void right_45(DC_motor *mL, DC_motor *mR, int count, int right_timer);

/*****************************************
* Function used to create a space between the buggy and the wall after a colour is read
*****************************************/
void space(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to define movements for yellow card while returning home
*****************************************/
void reverse_yellow(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to define movements for pink card while returning home
*****************************************/
void reverse_pink(DC_motor *mL, DC_motor *mR);

/*****************************************
* Function used to define movements when each card is read
* count input is used to define which card was identified and hence what move should be made
* Eg. movement(mL,mR,1) is used when red card is flagged and performs the required movements  
*****************************************/
void movement(DC_motor *mL, DC_motor *mR, int count);

/*****************************************
* Function used to define movements during RETURN HOME
* movements will be opposite to what was initially performed
 * Eg. movement_return(mL,mR,1) is used to perform the opposite of a green card   
*****************************************/
void movement_return(DC_motor *mL, DC_motor *mR, int count);

#endif
