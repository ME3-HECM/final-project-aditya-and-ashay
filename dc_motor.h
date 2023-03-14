#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#define _XTAL_FREQ 64000000

int left_timer = 105;
int right_timer = 100;


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
void initDCmotorsPWM(unsigned int PWMperiod); // function to setup PWM
void setMotorPWM(DC_motor *m);
void motor_init(DC_motor *mL,DC_motor *mR);
void buggyLEDs_init(void);

void forward(DC_motor *mL, DC_motor *mR);
void reverse(DC_motor *mL, DC_motor *mR);
void stop(DC_motor *mL, DC_motor *mR);

void left_45(DC_motor *mL, DC_motor *mR, int count, int left_timer);
void right_45(DC_motor *mL, DC_motor *mR, int count, int right_timer);
void space(DC_motor *mL, DC_motor *mR);

void instructions(DC_motor *mL, DC_motor *mR, int count);
void instructions2(DC_motor *mL, DC_motor *mR, int count);

#endif
