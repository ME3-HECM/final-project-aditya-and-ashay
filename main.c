// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "dc_motor.h"
#include "color.h"
#include "i2c.h"
#include "serial.h"
#include "interrupts.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    
    initDCmotorsPWM(99);
    
    struct DC_motor motorL, motorR; 		//declare two DC_motor structures 

    motorL.power=0; 						//zero power to start
    motorL.direction=1; 					//set default motor direction
    motorL.brakemode=1;						// brake mode (slow decay)
    motorL.posDutyHighByte=(unsigned char *)(&CCPR1H);  //store address of CCP1 duty high byte
    motorL.negDutyHighByte=(unsigned char *)(&CCPR2H);  //store address of CCP2 duty high byte
    motorL.PWMperiod=99; 			//store PWMperiod for motor (value of T2PR in this case)

    motorR.power=0; 						//zero power to start
    motorR.direction=1; 					//set default motor direction
    motorR.brakemode=1;						// brake mode (slow decay)
    motorR.posDutyHighByte=(unsigned char *)(&CCPR3H);  //store address of CCP3 duty high byte
    motorR.negDutyHighByte=(unsigned char *)(&CCPR4H);  //store address of CCP4 duty high byte
    motorR.PWMperiod=99; 			//store PWMperiod for motor (value of T2PR in this case)
    //same for motorR but different CCP registers
    
    // Sending colour data to realterm
    initUSART4();
    color_click_init();
    Interrupts_init();
    I2C_2_Master_Init();
    
    unsigned int R = color_read_Red();
    unsigned int B = color_read_Blue();
    unsigned int G = color_read_Green();
    unsigned int C = color_read_Clear();
    
    while (1) {
    char buf[40];
    sprintf(buf,"%d",R);
    sendStringSerial4(buf);
    }
    
//    for (int i = 0; i<=3; i += 1){
//    __delay_ms(1000);
//    fullSpeedAhead(&motorL,&motorR);
//    stop(&motorL,&motorR);
//    __delay_ms(1000);
//    turnLeft(&motorL,&motorR);
//    stop(&motorL,&motorR);
//    __delay_ms(1000);
//    }
//    
//    
//    turnLeft(&motorL,&motorR);
//    __delay_ms(1000);
//    
//    for (int j = 0; j<=3; j += 1){
//    __delay_ms(1000);
//    fullSpeedAhead(&motorL,&motorR);
//    stop(&motorL,&motorR);
//    __delay_ms(1000);
//    turnRight(&motorL,&motorR);
//    stop(&motorL,&motorR);
//    __delay_ms(1000);
//    }
//    
//    turnRight(&motorL,&motorR);
//    __delay_ms(1000);}
}