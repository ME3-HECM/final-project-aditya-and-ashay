#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"
#include "color.h"
#include "interrupts.h"
#include "serial.h"
#include "i2c.h"
#include "timers.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    
    initDCmotorsPWM(99);
    motor_init(&motorL,&motorR);
    buggyLEDs_init();
    initUSART4();
    color_click_init();
    Timer0_init();
    
    //colorclick_interrupts_init();
    //interrupts_init();
    
    TRISGbits.TRISG1 = 0; // Set TRIS value for red LED (output)
    TRISAbits.TRISA4 = 0; // Set TRIS value for green LED (output)
    TRISFbits.TRISF7 = 0; // Set TRIS value for blue LED (output)
    
    LATGbits.LATG1 = 1; //red LED on
    LATAbits.LATA4 = 1; //green LED on
    LATFbits.LATF7 = 1; //blue LED on
    
//    TRISFbits.TRISF2 = 1;
//    ANSELFbits.ANSELF2 = 0;
//    
//    TRISFbits.TRISF3 = 1;
//    ANSELFbits.ANSELF3 = 0;
    
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;
    
//    char start_flag = 0;
//    if (PORTFbits.RF3){
//        start_flag = 1;
//    }
//    if (start_flag = 1) {
//        timer_reset();
        
    while (1) {
       buggy_color_response(&motorL,&motorR,&color);
       }
    //start_flag = 0;
    //}

}        