# ECM final project - Mine navigation search and rescue - Aditya and Ashay 

## Table of contents
- [Challenge brief](#challenge-brief)
- [Instructions and environment specification](#instructions-and-environment-specification)
- [Demonstration video](#demonstration-video)
- [Hardware setup](#hardware-setup)
- [Calibrations](#calibrations)
- [Code structure](#code-structure)


## Challenge brief

Our task was to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Our robot buggy (named Alisha) was able to perform the following: 

1. Navigate towards a coloured card and ### stop before impacting the card
2. Read the card colour
3. Interpret the card colour using a predefined code and perform the navigation instruction
4. When the final card is reached, navigate back to the starting position
5. Handle exceptions and ### return back to the starting position if final card cannot be found

## Instructions and environment specification

A "mine" is contstructed from black plywood walls 100mm high with some walls having coloured cards located on the sides of the maze to assist with navigation. The following colour code was used for navigation:

Colour | Instruction
---------|---------
Red | Turn Right 90
Green | Turn Left 90
Blue | Turn 180
Yellow | Reverse 1 square and turn right 90
Pink | Reverse 1 square and turn left 90
Orange | Turn Right 135
Light blue | Turn Left 135 
White | Finish (return home)
Black | Maze wall colour

Mine courses varied in difficulty, with the simplest requiring 4 basic moves to navigate. More advanced courses required 10 or moves to navigate. The mines had features such as dead ends but colour cards always directed to the end of the maze. Once the end of the maze was reached, the buggy had to return to the starting position. 


## Demonstration video

The link(s) provided below show how the buggy performed in an 'easy' maze and a 'hard' maze. In addition, it also shows how the buggy responded to a dead end.

1. Link for the easy maze:
2. Link for the hard maze:
3. Link for dead end reaction:

## Hardware setup

An image of the buggy and its features are shown below.

![Buggy readme](https://user-images.githubusercontent.com/89412018/225479605-c1578c27-73a2-49b2-98e2-55520844e25c.JPG)

## Calibrations

One of the most important elements of this project was calibrating the buggy to dynamically varying surfaces and ambient light. This was necessary in order to perform the exact turns and read the colours perfectly. To assist in improving the buggy's performance, the rubber on the tyres were removed as it allowed more consistent turns when compared with the rubber on. There was also a special black sleeve which was 3-D printed to allow better colour reading. The calibration on test day was a 2 step process which went as follows:

###  Colour calibration

Colour calibration was performed once before the testing day and one during the test day to confirm if any changes were necessary in the code. To calibrate the colour, the Red, Green, Blue, and Clear values are read from the array of sensors built into the buggy's chip. The sensor contains a grid of 4x4 photodiodes, 4 are sensitive to red light, 4 green light, 4 blue light and 4 "clear" light (that is, a range of wavelengths, see datasheet for exact spectral response). When light falls on the photodiode the photons are absorbed and current is generated. This signal is then integrated over time using integrators and sampled by 16 bit on board ADCs. Communication with the device is achieved using an I2C interface. This enables configuration of the device to customise sampling of the data (i.e. integration time, gain, etc.) and to read the 16 bit digital values for each of the RGBC channels. The relative magnitude of these values gives you information about the colour of light that is falling on the sensor. 

We decided to have no gap between the buggy's sensors and the colour card so that the readings can be precise. We then measured over a 100 Red, Green, Blue and Clear values produced in multiple locations and time (day, night, evening, on the 7th floor, at home etc.) and stored these values in a text file with the help of the REALTERM software. 

We then imported these values in an excel sheet and for each colour we found normalised Red, Green and Blue values. This was done by dividing each individual colour by the Clear value and gave consistent results for multiple operating conditions. This process was repeated during test day with fewer data to confirm if the values fell into the right categories.

![image](https://user-images.githubusercontent.com/89412018/225482171-02faef63-39ad-4c15-9d02-0145cc8c0d68.png)

###
Turning calibration

Turning calibration was performed before every maze as the floor varied in roughness and surface level. Since the buggy did not have rotational encoders, it was imperative for the time to turn for each colour to be accurate. Our code for turning, which will be discussed in a later section, involves 45 degree turnes which are repeated to achieve 90, 135 and 180 degree turns. 

In order to implement a calibration function, we created a separate file called calibrations.c and added the functions in the main.c file which is commented out while performing in the actual maze. An example of the left turning functions is shown below:

```c
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
```

Here, the user lets the buggy turn 180 degrees in an anti-clockwise direction by clicking any of the butttons RF2 or RF3 on the buggy. It then performes the turn and the time it takes to rotate 45 x 4 can be adjusted by holding either the RF2 or RF3 button. The RF2 button increased the timer by 5ms and the RF3 button decreased the timer by 5ms incase the buggy overshoot the 180 degrees. A tally of increasing or decreasing button clicks are noted and the values are adjusted accordingly before the final test.

Link for turning calibration: 

## Code structure

### main.c 

The main.c file is used to initialise all the functions within the buggy and execute the code to explore the maze. All the initialisation functions are combined in the initialisation.c file and then implemented in the main.c file in order to have a clear and neat code. The main.c code also includes the calibration code which is commented out as it is executed separately from the final run.

```c
void main(void){
    
    complete_initialisation(); // Full initialisation of buggy function
   
    while (PORTFbits.RF2 && PORTFbits.RF3); //  empty while loop (wait for button press)
    
    if (!PORTFbits.RF3){__delay_ms(500);timer_reset();  // if button RF3 is pressed, wait for half a second before starting the car
    
        while (1) {
	
        buggy_color_response(&motorL,&motorR,&color); // execute maze exploring function 
	
        }
    }            
} 

//        battery_health(); // checks battery health
//
//        while (1) { //infinite while loop - repeat forever
//        
//        while (PORTFbits.RF2 && PORTFbits.RF3); // empty while loop (wait for button press)
//        
//        left_turn_calibration(&motorL,&motorR); // calibrate left 180 degree turns
//        
//        __delay_ms(2000); // give time to adjust delays within the calibration code

//        }
//}
```

### dcmotor.c

The dcmotor.c file is used to initialise the motors and define movements for the buggy. The motor is initialised using the motor_init function where the left and right motors are assigned addresses and PWM periods. 

```c
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
```

#### The basic movements are split into - forward, backward, stop, space, left_45 and right_45.

#### 1) Forward/Backward: 

This switches on the front lights/ real lights and sets power to left and right set of tyres to move in a straight line. This code will cause the car to move indefinitely in a particular direction but the color.c code adds a delay to allow the car to move specific distances.

Example of the forward code:

```c
void forward(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    mL-> direction = 1;
    mR-> direction = 1; // Forward direction
    MAINLIGHT = 1;
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <= 50) || (mR->power <= 55)){
        if (mL->power <= 50){mL->power += 1;}
        if (mR->power <= 55){mR->power += 1;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(5);   
    }
    MAINLIGHT = 0;
}
```
#### 2) Left/right 45 degrees: 

This code allows the buggy to turn clockwise or anti-clockwise in multiples of 45 degrees - i.e. 45, 90,135 and 180 degree turns. This is done by setting one set of motors to move forward while the other set turns backwards. A custom delay function is added to allow calibration and turn exactly 45 degrees in each loop.

```c
void left_45(DC_motor *mL, DC_motor *mR, int count, int left_timer)
{
    stop(mL,mR);       // Make sure motor is idle
    mL-> direction = 0;
    mR-> direction = 1; // moves right side 
    LEFTINDICATOR = 1;
    int i;
    for (i = 0;i<count;i++){ // counter to allow different turn angles
    while ((mL->power <= 50) || (mR->power <= 50)){
        if (mL->power <= 50){mL->power += 10;}
        if (mR->power <= 50){mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(10);  // increasing power in steps so that buggy doesn't slip
    }
    custom_delay(left_timer);
    stop(mL,mR); 
    __delay_ms(150); // Wait for Car to stabilise
    LEFTINDICATOR = 0;
    }
}
```
#### 3) Stop: 

This function is used to stop the buggy and allow it to stablise. It achieved the stop function by reducing the power applied to each well in increments until the power reaches 0.

```c
void space(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    HEADLAMPS = 1;
    mL-> direction = 0;
    mR-> direction = 0; // Reverse direction
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <30) && (mR->power <30)){
        mL->power += 10;
        mR->power += 10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);   
    }
    __delay_ms(350);
    stop(mL,mR);
    __delay_ms(200); // Wait for Car to stabilise
    HEADLAMPS = 0;  
}

```

#### 4) Space:

This function allows the buggy to maintain some space before performing the specific card colour function. It moves the buggy back by approximately 10 cm.

```c
void space(DC_motor *mL, DC_motor *mR)
{
    stop(mL,mR);
    HEADLAMPS = 1;
    mL-> direction = 0;
    mR-> direction = 0; // Reverse direction
    setMotorPWM(mR);
    setMotorPWM(mL);
    while ((mL->power <30) && (mR->power <30)){
        mL->power += 10;
        mR->power += 10;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_us(20);   
    }
    __delay_ms(350);
    stop(mL,mR);
    __delay_ms(200); // Wait for Car to stabilise
    HEADLAMPS = 0;
    
}
```


#### color.c


