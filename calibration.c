#include <xc.h>
#include "dc_motor.h"
#include "lights_buttons.h"


/************************************************************
* Function used to ease calibration of left turns
************************************************************/
void left_turn_calibration(DC_motor *mL, DC_motor *mR){
    while (!(!PORTFbits.RF3 && !PORTFbits.RF2)) {  //Styart calibration by pressing both buttons  
    left_45(mL,mR,4,left_timer); // do 180 degree turn (4 left_45 turns)
    __delay_ms(2000);
 
    if(!PORTFbits.RF2){left_timer  += 5;LEDD7 = 1;__delay_ms(200);LEDD7 = 0;} //if turn is not enough, press RF2 to increase delay time
    else if(!PORTFbits.RF3){left_timer  -= 5;LEDH3 = 1;__delay_ms(200);LEDH3 = 0;} //if turn is too much, press RF3 to decrease delay time
    __delay_ms(2000);   
    } 
}

/************************************************************
* Function used to ease calibration of right turns
************************************************************/
void right_turn_calibration(DC_motor *mL, DC_motor *mR){
    while (!(!PORTFbits.RF3 && !PORTFbits.RF2)) {    
        right_45(mL,mR,4,left_timer); // do 180 degree turn (4 left_45 turns)
        __delay_ms(2000);
            
        if(!PORTFbits.RF2){right_timer  += 5;LEDD7 = 1;__delay_ms(200);LEDD7 = 0;} //if turn is not enough, press RF2 to increase delay time
        else if(!PORTFbits.RF3){right_timer  -= 5;LEDH3 = 1;__delay_ms(200);LEDH3 = 0;} //if turn is too much, press RF3 to decrease delay time
        __delay_ms(2000);    
        } 
}

/************************************************************
* Function used to create a custom delay time based on a variable input
************************************************************/
void custom_delay(int temp){
int i;
for (i = 0; i < temp; i++){__delay_ms(1);}
}

/************************************************************
* Function used to identify whether batteyr must be charged or not
* Eg. battery_health() will flash lights if battery health reading is below 160
************************************************************/
void battery_health(void) {
    unsigned char battery_level;
    if (battery_level < 160) {
        while (1){
            LEDD7 = !LEDD7;
            LEDH3 = !LEDH3;
        }
    } 
}

