#include <xc.h>
#include "dc_motor.h"
#include "lights_buttons.h"



void left_turn_calibration(DC_motor *mL, DC_motor *mR){
    while (!(!PORTFbits.RF3 && !PORTFbits.RF2)) {    
    left_45(mL,mR,4,left_timer);
    __delay_ms(2000);
        // Note: Hold on pressing your desired buttons until LED flashes on
    if(!PORTFbits.RF2){left_timer  += 5;LEDD7 = 1;__delay_ms(200);LEDD7 = 0;} 
    else if(!PORTFbits.RF3){left_timer  -= 5;LEDH3 = 1;__delay_ms(200);LEDH3 = 0;}
    __delay_ms(2000); // Leave time to exit Calibration   
    } 
}

void right_turn_calibration(DC_motor *mL, DC_motor *mR){
    while (!(!PORTFbits.RF3 && !PORTFbits.RF2)) {    
        right_45(mL,mR,4,left_timer);
        __delay_ms(2000);
            // Note: Hold on pressing your desired buttons until LED flashes on
        if(!PORTFbits.RF2){right_timer  += 5;LEDD7 = 1;__delay_ms(200);LEDD7 = 0;} 
        else if(!PORTFbits.RF3){right_timer  -= 5;LEDH3 = 1;__delay_ms(200);LEDH3 = 0;}
        __delay_ms(2000); // Leave time to exit Calibration   
        } 
}

void custom_delay(int temp){
int i;
for (i = 0; i < temp; i++){__delay_ms(1);}
}

//void reverse_square_time();
