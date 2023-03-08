#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

#define HEADLAMPS_LED LATHbits.LATH1 // Toggles the front white LEDs and rear red LEDs (at a reduced brightness)
#define MAINBEAM_LED LATDbits.LATD3  // Toggles the front white LEDs (at full brightness)
#define BRAKE_LED LATDbits.LATD4     // Toggles the rear red LEDs (at full brightness)
#define TURNLEFT_LED LATFbits.LATF0  // Toggles the left signal (but no hardware based brightness control)
#define TURNRIGHT_LED LATHbits.LATH0 // Toggles the right signal (but no hardware based brightness control)


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


void left_45(DC_motor *mL, DC_motor *mR);
void right_45(DC_motor *mL, DC_motor *mR);


#endif
