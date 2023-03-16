# ECM final project - Mine navigation search and rescue - Aditya and Ashay 

## Table of contents
- [Challenge brief](#challenge-brief)
- [Instructions and environment specification](#instructions-and-environment-specification)
- [Demonstration video](#demonstration-video)
- [Hardware setup](#hardware-setup)
- [Calibrations](#calibrations)
- [Code structure](#code-structure)


## Challenge brief

Our task was to develop an autonomous robot that can navigate a "mine" using a series of instructions coded in coloured cards and return to its starting position.  Our robot buggy was able to perform the following: 

1. Navigate towards a coloured card 
2. Read the card colour
3. Interpret the card colour using a predefined code and perform the navigation instruction
4. When the final card is reached, navigate back to the starting position
5. Handle exceptions and return back to the starting position if final card cannot be found

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

1. Link for multiple mazes: https://imperiallondon-my.sharepoint.com/:f:/g/personal/at2720_ic_ac_uk/ErTWnWZFvw5Lom3iuJFQY-4BAJXrrrWgnjZK5RJU2tOjOA?e=nCbbrg

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

### Turning calibration

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

Link for turning calibration: https://imperiallondon-my.sharepoint.com/:f:/g/personal/at2720_ic_ac_uk/Etbmkv8cqCxIpUSVXiz-qoABaZUonyaD3vTknrmlewBVGA?e=jWhfkU

## Code structure

### 1) main.c 

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

### 2) dcmotor.c

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

#### The basic movements are split into - forward, backward, stop, space, left_45 and right_45. These are then compounded for each colour as mentioned in the'Instructions and environment specification'.

#### a) Forward/Backward: 

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
#### b) Left/right 45 degrees: 

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
#### c) Stop: 

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

#### d) Space:

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
#### e) Compound movements:

This function stores each colour as a number - from 1 being red to 8 being white and executes a set of codes as per the instructions. Before the appropriate insturction takes place, the buggy uses the space function to get some distance to turn. There is another function aclled movement2 which is executed once the white flag is reached. This allows the buggy to move back with the help of timers and doesn't require the space function.

```c
void movement(DC_motor *mL, DC_motor *mR, int count)
{
    space(mL,mR);
    __delay_ms(500);
    stop(mL,mR);
    __delay_ms(500);
    if (count == 1){right_45(mL,mR,2,right_timer); stop(mL,mR);}
    if (count == 2){left_45(mL,mR,2,left_timer); stop(mL,mR);}
    if (count == 3){right_45(mL,mR,4,right_timer); stop(mL,mR);}
    if (count == 4){reverse(mL,mR); __delay_ms(900);stop(mL,mR);__delay_ms(500);right_45(mL,mR,2,right_timer); stop(mL,mR);}
    if (count == 5){reverse(mL,mR); __delay_ms(900);stop(mL,mR);__delay_ms(500);left_45(mL,mR,2,left_timer); stop(mL,mR);}
    if (count == 6){right_45(mL,mR,3,right_timer); stop(mL,mR);}
    if (count == 7){left_45(mL,mR,3,left_timer); stop(mL,mR);}
          
}
```


### 3) color.c

This file has functions which calculate, execute and store each movement done by the buggy.

#### a) Colour reading functions:

There were functions provided which read the Red, Green, Blue and Clear values from the sensors. We then added a code called READcolor() which would take these values everytime the function was called and would normalise the values. This was done due to the calibration of the sensors and provided consistent results. 

```c
void READcolor(colors *c) {
    
    color.R = color_read_Red();
    color.B = color_read_Blue();
    color.G = color_read_Green();
    color.C = color_read_Clear();
    
    color.R_norm = (float)color.R / (float)color.C;
    color.B_norm = (float)color.B / (float)color.C;
    color.G_norm = (float)color.G / (float)color.C;
}
```

#### b) Buggy response

This was one of the main functions we wrote. This code would read the colour values and carry out the particular instruction. It would further store timer values and colour values in separate arrays to allow the buggy to return to the start point after reading the white colour.

- Timer_memory: stores the forward movement time between colours instructions 
- card_memory: stores the appropriate return response after reading a color. For instance red colour means turn right 90 degree. The code will store the value for color green i.e. 2 which means that when the array is read, it will turn left 90 degrees.

The code is quite lengthy and thus will be explained in simplified parts:

```c
void buggy_color_response(DC_motor *mL, DC_motor *mR, colors *c) {
    
    READcolor(&color);
    
    if (color.C > color_upperbound){ 
        timer_memory[timer_index] = timer_val;
        timer_index ++;
        forward(mL,mR);
        __delay_ms(50);
        stop(mL,mR); __delay_ms(500);
        READcolor(&color); __delay_ms(500);
        
        if (color.R_norm > 0.77 && color.B_norm < 0.18 && color.G_norm < 0.14){
            card_memory[card_count] = 2;
            card_count ++;
            movement(mL,mR,1); //RED COLOUR CARD - Turn Right
         
        }
```

The ambient lighting has really low Clear values and thus as soon as the value exceeds 2500, a color is in the vicinity. The code then reads the color, stores the forward movement time and moves the buggy a bit more forward. This is to allow the buggy to self align with the wall. It then reads the color again to double confirm and carries out mathematical operations to see which color it is. It then accordingly stores the opposite insturction value as the arrays are read backwards and all movements are opposite while returing.

We also calibrated the colours of the wall to implement a lost function. When a wall is hit, the buggy moves back and forth and reads the colour detelcted. If the wall colour is detected 5 times, it confirms that the buggy is lost and the return home function is implemented.

```c
else if(color.C < 2200 && color.B < 500 && color.B_norm < 0.24) { // Wall Colour (LOST)
                    
            card_memory[card_count] = 3; //append 3 to array so the first move performed when return_home() is called is a 180 turn
            card_count ++; 
            char count2 = 0;
            
            for (char j = 0; j <= 5; j++){   // buggy move back and forth 5 times and checks if it is still hitting the wall 
                reverse(mL,mR);
                __delay_ms(200);
                stop(mL,mR);
                __delay_ms(200);
                forward(mL,mR);
                __delay_ms(200);
                READcolor(&color);
                if (color.C < 2200 && color.B < 500 && color.B_norm < 0.24){count2 += 1;} // adds 1 to the counter every time it detects the wall
            }
            
            if (count2 == 5){  //when 5 wall readings are confirmed, buggy is confirmed 'lost' and will return home

                LATGbits.LATG1 = 0; //red LED OFF
                LATAbits.LATA4 = 0; //green LED OFF
                LATFbits.LATF7 = 0; //blue LED OFF (Allows us to see if white card has been triggered)

                space(mL,mR); //Create space between wall and buggy
                __delay_ms(500);
                stop(mL,mR);
                __delay_ms(500);

                return_home(mL,mR); // iterate through arrays to bring buggy back to start

                stop(mL,mR); 
                __delay_ms(500);

                movement_return(mL,mR,3); //Lastly turn 180 so buggy is in exact start position
                Sleep(); // once at start position stop the code from running 
    
            }           
```

Similarly, the code for white flag is:

```c
 if (color.R_norm < 0.5 && color.C > 16000  ){ //White card - Return home
           
                
            card_memory[card_count] = 3;
            card_count ++;
            
            LATGbits.LATG1 = 0; //red LED OFF
            LATAbits.LATA4 = 0; //green LED OFF
            LATFbits.LATF7 = 0; //blue LED OFF
                
            space(mL,mR);
            __delay_ms(500);
            stop(mL,mR);
            __delay_ms(500);
            
            return_home(mL,mR);
            
            stop(mL,mR);
            __delay_ms(500);
            
            movement_return(mL,mR,3);
            Sleep();   
        
        }
	    else {forward(mL,mR);} //If clear channel is below 2500, car will continue to move forward
}
```
When the white color is detected, the code switches off the LED, maintains space and turns around 180 degrees. It then follows the return_home function which reads the array values until it reaches the end. It then stops, turns around and puts the buggy to sleep. If no colour is found the buggy just moves forward endlessly.

```c
void return_home(DC_motor *mL, DC_motor *mR){
    while(timer_index > 0 && card_count > 0 ) {
        
        movement_return(mL,mR,card_memory[card_count-1]);
        card_count--;
        __delay_ms(250);
        reverse(mL,mR);
        __delay_ms(500);
        stop(mL,mR);
        __delay_ms(500);
        forward(mL,mR);
        delay_ms_func(timer_memory[timer_index-1]); //-2 corresponds to 262ms and accounts for the space created between white card and wall
        timer_index--; 
        stop(mL,mR);
        __delay_ms(200);
         reverse(mL,mR);
        __delay_ms(100);
        stop(mL,mR);        
        __delay_ms(250);

    }
}
```

This function implements array reading and buggy alignment. It reads the color array from the last value - last in first out (LIFO). It then has a small function where the buggy purposely moves backwards for 0.5s in order to align it with the wall and allow efficient movement along the maze. This was proven to be more effective than other methods and we decided to stick with it. It then executes the forward movement with delays read from the timer array - last in first out but with a slightly reduced time (approximated 250ms) to account for the space function. It is also preceeded with a small reverse function incase the buggy hits a wall on the way.
