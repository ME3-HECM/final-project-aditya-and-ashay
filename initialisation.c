#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "interrupts.h"
#include "serial.h"
#include "i2c.h"
#include "timers.h"
#include "lights_buttons.h"
#include "calibration.h"
#include "initialisation.h"


void need4speed(void){
    initDCmotorsPWM(99);
    buggyLEDs_init();
    ports_init();
    motor_init(&motorL,&motorR);
    I2C_2_Master_Init();
    color_click_init();
    Timer0_init();
    interrupts_init();
    initUSART4();
    
    //colorclick_interrupts_init();
}
