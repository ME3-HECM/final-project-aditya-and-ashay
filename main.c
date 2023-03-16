#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "interrupts.h"
#include "timers.h"
#include "lights_buttons.h"
#include "calibration.h"
#include "initialisation.h"

#define _XTAL_FREQ 64000000   

void main(void){
    
    complete_initialisation(); //Full initialisation of buggy function (Found in initilisation.c)
   
    while (PORTFbits.RF2 && PORTFbits.RF3); //Empty while loop (Wait for button press)
    
    /*****************Start the maze when RF3 is pressed*************/
    
    if (!PORTFbits.RF3){__delay_ms(500);timer_reset();   //If button RF3 is pressed wait for half a second before starting buggy
        
    while (1) {
            
        buggy_color_response(&motorL,&motorR,&color); //Execute maze exploring function (Found in color.c)
        
        }
    }            
} 
/********************** Calibration code ran prior to final maze run*****************/

//        battery_health(); // checks battery health (Found in calibration.c)

//        while (1) { 
//        
//        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop (wait for button press)
//        
//        left_turn_calibration(&motorL,&motorR); //calibrate left 180 degree turns (Found in calibration.c))
//        
//        __delay_ms(2000); // give time to adjust delays within the calibration code
//        }
//}