#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and CCP for DC motor control
void initDCmotorsPWM(unsigned int PWMperiod){
    //initialise your TRIS and LAT registers for PWM  
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE4 = 0;
    TRISCbits.TRISC7 = 0;
    TRISGbits.TRISG6 = 0;
    
    LATEbits.LATE2 = 0;
    LATEbits.LATE4 = 0;
    LATCbits.LATC7 = 0;
    LATGbits.LATG6 = 0;
    
    //configure PPS to map CCP modules to pins
    RE2PPS=0x05; //CCP1 on RE2
    RE4PPS=0x06; //CCP2 on RE4
    RC7PPS=0x07; //CCP3 on RC7
    RG6PPS=0x08; //CCP4 on RG6

    // timer 2 configuration
    T2CONbits.CKPS=0b100; // 1:16 prescaler 0b110 is 8 bit
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER or T2PR
    // 0.0001s*16MHz/16 -1 = 99. T2PR must be 99 then... (CONFUSION)
    T2PR=PWMperiod; //Period reg 10kHz base period, PMWperiod must be 99 for it to have 10kHz
    T2CONbits.ON=1;
    
    //setup CCP modules to output PMW signals
    //initial duty cycles (0% duty or 100% duty)
    CCPR1H=0; 
    CCPR2H=0; 
    CCPR3H=0; 
    CCPR4H=0; 
    
    //use tmr2 for all CCP modules used
    CCPTMRS0bits.C1TSEL=0;
    CCPTMRS0bits.C2TSEL=0;
    CCPTMRS0bits.C3TSEL=0;
    CCPTMRS0bits.C4TSEL=0;
    
    //configure each CCP
    CCP1CONbits.FMT=1; // left aligned duty cycle (we can just use high byte)
    CCP1CONbits.CCP1MODE=0b1100; //PWM mode  
    CCP1CONbits.EN=1; //turn on
    
    
    CCP2CONbits.FMT=1; // left aligned
    CCP2CONbits.CCP2MODE=0b1100; //PWM mode  
    CCP2CONbits.EN=1; //turn on
    
    
    
    CCP3CONbits.FMT=1; // left aligned
    CCP3CONbits.CCP3MODE=0b1100; //PWM mode  
    CCP3CONbits.EN=1; //turn on
    
    
    CCP4CONbits.FMT=1; // left aligned
    CCP4CONbits.CCP4MODE=0b1100; //PWM mode  
    CCP4CONbits.EN=1; //turn on
    
}


//TRY TO TURN IN SMALL INCREMENTS AND HAVE A CALIBRATION FUNCTION!!!!!!!!!!
// function to set CCP PWM output from the values in the motor structure
void setMotorPWM(DC_motor *m)
{
    unsigned char posDuty, negDuty; //duty cycle values for different sides of the motor
    
    if(m->brakemode) { //Slow decay - Brake
        posDuty = m->PWMperiod - ((unsigned int)(m->power)*(m->PWMperiod))/100; //inverted PWM duty
        negDuty = m->PWMperiod; //other side of motor is high all the time
    }
    else { //Fast decay - Coast
        posDuty=0; //other side of motor is low all the time
		negDuty=((unsigned int)(m->power)*(m->PWMperiod))/100; // PWM duty
    }
    
    if (m->direction) {
        *(m->posDutyHighByte)= posDuty;  //assign values to the CCP duty cycle registers
        *(m->negDutyHighByte)= negDuty;       
    } else {
        *(m->posDutyHighByte)= negDuty;  //do it the other way around to change direction
        *(m->negDutyHighByte)= posDuty;
    }
}

//function to stop the robot gradually 
void stop(DC_motor *mL, DC_motor *mR)
{
    mL->brakemode = 1;
    mR->brakemode = 1;
    
    for (int i = 70; i >= 0; i=i-10) {
        mL -> power = i;
        mR -> power = i;  
        setMotorPWM(mL);
        setMotorPWM(mR); 
        __delay_us(20);
    }
    
    mL->power = 0;
    mR->power = 0;
    setMotorPWM(mL);
    setMotorPWM(mR);
}

//function to make the robot turn left 
void turnLeft(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);       // Make sure motor is idle
    mL-> direction = 0;
    mR-> direction = 1; // moves right side 
    
    while ((mL->power <= 30) || (mR->power <= 30)){
        if (mL->power <= 30){mL->power += 10;}
        if (mR->power <= 30){mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);
    }
    __delay_ms(425);
    
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
}

//function to make the robot turn right 
void turnRight(DC_motor *mL, DC_motor *mR, int deg)
{
    char i;
    i = deg/45;
    
    char j;
    for (j=0;j<i;j++) {
    mL-> direction = 1;
    mR-> direction = 0; // moves right side 
    while ((mL->power <= 30) || (mR->power <= 30)){
        if (mL->power <= 30){mL->power += 10;}
        if (mR->power <= 30){mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(50);
    }
    __delay_ms(230); // delay for 45 degree 
    }
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
}

//function to make the robot go straight
void fullSpeedAhead(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    mL-> direction = 1;
    mR-> direction = 1; // Forward direction
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <40) && (mR->power <40)){
        mL->power += 10;
        mR->power += 10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);   
    }
    __delay_ms(2000);
}