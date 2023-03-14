#ifndef CALIBRATION_H
#define	CALIBRATION_H

#include <xc.h>

#define _XTAL_FREQ 64000000

void left_turn_calibration(DC_motor *mL, DC_motor *mR);
void right_turn_calibration(DC_motor *mL, DC_motor *mR);
void custom_delay(int temp);


#endif	
