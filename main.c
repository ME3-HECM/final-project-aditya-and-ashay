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

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    
    need4speed();
   
    
    //char start_flag = 0;
    while (PORTFbits.RF2 && PORTFbits.RF3);
    if (!PORTFbits.RF3){timer_reset();
        while (1) {
        buggy_color_response(&motorL,&motorR,&color);
        }}        
    
//    if (start_flag = 1) {
//        timer_reset();
//        while (1) {
//        buggy_color_response(&motorL,&motorR,&color);
//        }
    } 


//        while (1) { //infinite while loop - repeat forever
//        
//        while (PORTFbits.RF2 && PORTFbits.RF3); //empty while loop (wait for button press)
//        
//        left_turn_calibration(&motorL,&motorR);
//        
//        __delay_ms(2000); 
//        }
//}