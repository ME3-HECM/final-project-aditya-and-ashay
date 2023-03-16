#include <xc.h>
#include "dc_motor.h"
#include "lights_buttons.h"
#include "calibration.h"



/************************************************************
* Function used to initialise T2 and CCP for DC motor control
************************************************************/
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

/**********************************************************************
* Function to set CCP PWM output from the values in the motor structure
**********************************************************************/
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


/*********************************************
* Function to initialise motor specifications 
*********************************************/
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


/************************************
* Function used to move buggy forward
************************************/
void forward(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR); // Make sure motor is idle
    mL-> direction = 1;
    mR-> direction = 1; // Forward direction
    MAINLIGHT = 1; //Turn main beam lights on
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <= 50) || (mR->power <= 55)){
        if (mL->power <= 50){mL->power += 1;} 
        if (mR->power <= 55){mR->power += 1;} //Gradual power increase
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(5);   
    }
    MAINLIGHT = 0; //Turn main beam lights off after move is performed
}

/*****************************************************
* Function used to move buggy in the reverse direction
*****************************************************/
void reverse(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR); // Make sure motor is idle
    mL-> direction = 0;
    mR-> direction = 0; // Reverse direction
    HEADLAMPS = 1; //Turn reverse lights on
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <= 50) || (mR->power <= 55)){
        if (mL->power <= 50){mL->power += 1;} //Gradual power increase
        if (mR->power <= 55){mR->power += 1;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(5);   
    }
    HEADLAMPS = 0; //Turn reverse lights off after move is performed
}

/*****************************************
* Function used to stop buggy gradually
*****************************************/
void stop(DC_motor *mL, DC_motor *mR)
{
    mL->brakemode = 1;
    mR->brakemode = 1; //mode defined for braking to occur
    BRAKELIGHT = 1;
    
    for (char i = 70; i >= 0; i=i-10) {
        mL -> power = i;
        mR -> power = i;  // Power loss is gradual
        setMotorPWM(mL);
        setMotorPWM(mR); 
        __delay_us(20);
    }
    
    mL->power = 0;
    mR->power = 0; //0 power causes motors to come to a stop
    setMotorPWM(mL);
    setMotorPWM(mR);
    BRAKELIGHT = 0;
}

/*****************************************
* Function used to make buggy turn left 45 degrees
* count input defines how many 45 degree turns the function will perform 
* left_45(mL,mR,2,left_timer) - This will do two 45 degree turns therefore make a 90 degree turn
*****************************************/
void left_45(DC_motor *mL, DC_motor *mR, char count, int left_timer)
{
    stop(mL,mR); // Make sure motor is idle
    mL-> direction = 0; //moves left side backward
    mR-> direction = 1; // moves right side forward 
    LEFTINDICATOR = 1;
    char i;
    for (i = 0;i<count;i++){
    while ((mL->power <= 50) || (mR->power <= 50)){
        if (mL->power <= 50){mL->power += 10;}
        if (mR->power <= 50){mR->power += 10;} //Gradual power increase
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);
    }
    custom_delay(left_timer); //delay time set by calibration, delay set to perform an accurate 45 degree turn
    stop(mL,mR); // stop buggy after move is performed
    __delay_ms(150); // Wait for Car to stabilise
    LEFTINDICATOR = 0;
    }
}

/*****************************************
* Function used to make buggy turn right 45 degrees
* count input defines how many 45 degree turns the function will perform  
* right_45(mL, mR, 2, right_timer) - This will do two 45 degree turns therefore make a 90 degree turn
*****************************************/
void right_45(DC_motor *mL, DC_motor *mR, char count, int right_timer)
{
    stop(mL,mR); // Make sure motor is idle
    mL-> direction = 1; //moves left side forward
    mR-> direction = 0; // moves right side backward
    RIGHTINDICATOR = 1;
    char i;
    for (i = 0;i<count;i++){
    while ((mL->power <= 50) || (mR->power <= 50)){
        if (mL->power <= 50){mL->power += 10;}
        if (mR->power <= 50){mR->power += 10;} //Gradual power increase
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);   
    }
    custom_delay(right_timer); //delay time set by calibration, delay set to perform an accurate 45 degree turn
    stop(mL,mR); // stop buggy after move is performed
    __delay_ms(150); // Wait for Car to stabilise
    RIGHTINDICATOR = 0;
    }
}

/*****************************************
* Function used to create a space between the buggy and the wall after a colour is read
*****************************************/
void space(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR); // Make sure motor is idle
    HEADLAMPS = 1;
    mL-> direction = 0;
    mR-> direction = 0; // Reverse direction
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <30) && (mR->power <30)){
        mL->power += 10;
        mR->power += 10; //Gradual power increase
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);   
    }
    __delay_ms(350); //Buggy will reverse for this length of time
    stop(mL,mR); // stop buggy after move is performed
    __delay_ms(200); // Wait for Car to stabilise
    HEADLAMPS = 0;
    
}

/*****************************************
* Function used to define movements for yellow card while returning home
*****************************************/
void reverse_yellow(DC_motor *mL, DC_motor *mR) {
    
    left_45(mL,mR,2,left_timer); // 90 degree left turn
    stop(mL,mR);
    __delay_ms(200);
    
     reverse(mL,mR);
    custom_delay(square_timer); //square_timer set by calibration; defines time to reverse 1 square
    stop(mL,mR); // stop buggy after move is performed
}

/*****************************************
* Function used to define movements for pink card while returning home
*****************************************/
void reverse_pink(DC_motor *mL, DC_motor *mR) {
   
    right_45(mL,mR,2,right_timer); // 90 degree right turn
    stop(mL,mR);
    __delay_ms(200);
    
    reverse(mL,mR);
    custom_delay(square_timer); //square_timer set by calibration; defines time to reverse 1 square 
    stop(mL,mR); // stop buggy after move is performed
}

/*****************************************
* Function used to define movements when each card is read
* count input is used to define which card was identified and hence what move should be made
* Eg. movement(mL,mR,1) is used when red card is flagged and performs the required movements  
*****************************************/
void movement(DC_motor *mL, DC_motor *mR, char count)
{
    space(mL,mR); //Reverses away from wall
    __delay_ms(500);
    stop(mL,mR); //stabilises buggy
    __delay_ms(500);
    
    if (count == 1){right_45(mL,mR,2,right_timer); stop(mL,mR);} // RED
    if (count == 2){left_45(mL,mR,2,left_timer); stop(mL,mR);} // GREEN
    if (count == 3){right_45(mL,mR,4,right_timer); stop(mL,mR);} //BLUE
    if (count == 4){reverse(mL,mR); custom_delay(square_timer);stop(mL,mR);__delay_ms(500);right_45(mL,mR,2,right_timer); stop(mL,mR);} //YELLOW
    if (count == 5){reverse(mL,mR); custom_delay(square_timer);stop(mL,mR);__delay_ms(500);left_45(mL,mR,2,left_timer); stop(mL,mR);} //PINK
    if (count == 6){right_45(mL,mR,3,right_timer); stop(mL,mR);} //ORANGE
    if (count == 7){left_45(mL,mR,3,left_timer); stop(mL,mR);} //LIGHT BLUE
       
}

/*****************************************
* Function used to define movements during RETURN HOME
* movements will be opposite to what was initially performed
 * Eg. movement_return(mL,mR,1) is used to perform the opposite of a green card   
*****************************************/
void movement_return(DC_motor *mL, DC_motor *mR, char count)
{
       
    if (count == 1){right_45(mL,mR,2,right_timer); stop(mL,mR);} //GREEN (RETURN HOME)
    if (count == 2){left_45(mL,mR,2,left_timer); stop(mL,mR);}//RED (RETURN HOME)
    if (count == 3){right_45(mL,mR,4,right_timer); stop(mL,mR);}//BLUE - same as before
    if (count == 6){right_45(mL,mR,3,right_timer); stop(mL,mR);}//LIGHT BLUE (RETURN HOME)
    if (count == 7){left_45(mL,mR,3,right_timer); stop(mL,mR);}//ORANGE (RETURN HOME)
        
    if (count == 9){reverse_yellow(mL,mR);} //YELLOW (RETURN HOME)
    if (count == 10) {reverse_pink(mL,mR);} //PINK (RETURN HOME)
}