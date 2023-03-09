// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    
    initDCmotorsPWM(99);
    buggyLEDs_init();
    motor_init(&motorL,&motorR);

    
    __delay_ms(1000);
    instructions(&motorL,&motorR,1);
    __delay_ms(1000);
    instructions(&motorL,&motorR,2);
    __delay_ms(1000);
    instructions(&motorL,&motorR,3);
    __delay_ms(1000);
    instructions(&motorL,&motorR,4);
    __delay_ms(1000);
    instructions(&motorL,&motorR,5);
    __delay_ms(1000);
    instructions(&motorL,&motorR,6);
    __delay_ms(1000);
    instructions(&motorL,&motorR,7);
    __delay_ms(1000);
}
