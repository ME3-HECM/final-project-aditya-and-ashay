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

void motor_init(DC_motor *mL, DC_motor *mR)
{
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
    
}

void buggyLEDs_init(void) {
    HEADLAMPS_LED = 0;    // Set initial LAT value for H.LAMPS
    MAINBEAM_LED = 0;     // Set initial LAT value for M.BEAM
    BRAKE_LED = 0;        // Set initial LAT value for BRAKE
    TURNLEFT_LED = 0;     // Set initial LAT value for TURN-L
    TURNRIGHT_LED = 0;    // Set initial LAT value for TURN-R
    
    TRISHbits.TRISH1 = 0; // Set TRIS value for H.LAMPS
    TRISDbits.TRISD3 = 0; // Set TRIS value for M.BEAM
    TRISDbits.TRISD4 = 0; // Set TRIS value for BRAKE
    TRISFbits.TRISF0 = 0; // Set TRIS value for TURN-L
    TRISHbits.TRISH0 = 0; // Set TRIS value for TURN-R
}

//function to make the robot go straight
void forward(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    MAINBEAM_LED = 1;
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
    MAINBEAM_LED = 0;
}

void reverse(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    MAINBEAM_LED = 1;
    mL-> direction = 0;
    mR-> direction = 0; // Forward direction
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
    MAINBEAM_LED = 0;
}

//function to stop the robot gradually 
void stop(DC_motor *mL, DC_motor *mR)
{
    mL->brakemode = 1;
    mR->brakemode = 1;
    BRAKE_LED = 1;
    
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
    BRAKE_LED = 0;
}

//function to make the robot turn left 
void left_45(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);       // Make sure motor is idle
    mL-> direction = 0;
    mR-> direction = 1; // moves right side 
    TURNLEFT_LED = 1;
    
    while ((mL->power <= 30) || (mR->power <= 30)){
        if (mL->power <= 30){mL->power += 10;}
        if (mR->power <= 30){mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);
    }
    __delay_ms(210);
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
    TURNLEFT_LED = 0;
}

//function to make the robot turn right 
void right_45(DC_motor *mL, DC_motor *mR)
{
    mL-> direction = 1;
    mR-> direction = 0; // moves right side 
    while ((mL->power <= 30) || (mR->power <= 30)){
        if (mL->power <= 30){mL->power += 10;}
        if (mR->power <= 30){mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(50);   
    }
    __delay_ms(215);
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
}
