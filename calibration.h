#ifndef CALIBRATION_H
#define	CALIBRATION_H

#include <xc.h>

#define _XTAL_FREQ 64000000

/************************************************************
* Function used to ease calibration of left turns
************************************************************/
void left_turn_calibration(DC_motor *mL, DC_motor *mR);

/************************************************************
* Function used to ease calibration of right turns
************************************************************/
void right_turn_calibration(DC_motor *mL, DC_motor *mR);

/************************************************************
* Function used to create a custom delay time based on a variable input
************************************************************/
void custom_delay(int temp);

/************************************************************
* Function used to identify whether batteyr must be charged or not
* Eg. battery_health() will flash lights if battery health reading is below 160
************************************************************/
void battery_health(void);


#endif	