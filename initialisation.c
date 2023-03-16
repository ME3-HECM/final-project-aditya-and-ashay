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
#include "ADC.h"

/************************************************************
* Function used to initiliase all the functions that must be initialised before buggy can enter the maze
************************************************************/
void complete_initialisation(void){
    
    initDCmotorsPWM(99); 
    
    buggyLEDs_init();
    
    ports_init(); 
    
    motor_init(&motorL,&motorR);
    
    I2C_2_Master_Init();
    
    color_click_init();
    
    Timer0_init();
    
    interrupts_init();
    
    initUSART4();
    
    ADC_init();
    
    //Done to simplify code in main.c
}
