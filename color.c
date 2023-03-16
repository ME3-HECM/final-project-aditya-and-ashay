#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#include "color.h"
#include "i2c.h"
#include "dc_motor.h"
#include "timers.h"
#include "interrupts.h"
#include "lights_buttons.h"

/*****************************************
* Function used to initialise colour click
*****************************************/
void color_click_init(void)
{   
    //setup colour sensor via i2c interface
    I2C_2_Master_Init();      //Initialise i2c Master
     //set device PON
	 color_writetoaddr(0x00, 0x01);
    __delay_ms(3); //need to wait 3ms for everthing to start up
    //turn on device ADC
	color_writetoaddr(0x00, 0x03);
    //set integration time
	color_writetoaddr(0x01, 0xD5);
}
/*****************************************
* Function used to write to the colour click
*****************************************/
void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

/*****************************************
* Function used to read the red light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    // For line below Red = 0x16, Blue = 0x1A (0x01 according to Amanda), Green = 0x18, Clear = 0x14
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*****************************************
* Function used to read the blue light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Blue(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    // For line below Red = 0x16, Blue = 0x1A (0x01 according to Amanda), Green = 0x18, Clear = 0x14
	I2C_2_Master_Write(0xA0 | 0x1A);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*****************************************
* Function used to read the green light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Green(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    // For line below Red = 0x16, Blue = 0x1A (0x01 according to Amanda), Green = 0x18, Clear = 0x14
	I2C_2_Master_Write(0xA0 | 0x18);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*****************************************
* Function used to read the clear light and return a 16 bit colour intensity reading
*****************************************/
unsigned int color_read_Clear(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    // For line below Red = 0x16, Blue = 0x1A (0x01 according to Amanda), Green = 0x18, Clear = 0x14
	I2C_2_Master_Write(0xA0 | 0x14);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

/*****************************************
* Function used to perform colour reads and assign them to struct variables
* normalised values are also added to the colour struct 
*****************************************/
void READcolor(colors *c) {
    
    color.R = color_read_Red();
    color.B = color_read_Blue();
    color.G = color_read_Green();
    color.C = color_read_Clear();
    
    color.R_norm = (float)color.R / (float)color.C; 
    color.B_norm = (float)color.B / (float)color.C;
    color.G_norm = (float)color.G / (float)color.C; //normalised values found by: (colour intensity / clear channel intensity)
}

/*****************************************
* Function used to read colours and create the desired response of the buggy
* Function also assigns colour read and timer values to an array to  be used in the return home function
*****************************************/
void buggy_color_response(DC_motor *mL, DC_motor *mR, colors *c) {
    
    READcolor(&color); //Read colour intensities initially
    
    if (color.C > color_upperbound){ //if clear channel value is above certain value it means a card has been reached
        timer_memory[timer_index] = timer_val; // First append the timer value to timer array to be used while return home
        timer_index ++; //increment timer index so the next position in the array can be accessed next time a card is flagged
        forward(mL,mR); //move forward to ensure buggy is hitting the card and read is accurate
        __delay_ms(50);
        stop(mL,mR); __delay_ms(500);
        READcolor(&color); __delay_ms(500); // Read colours again
        
        if (color.R_norm > 0.77 && color.B_norm < 0.18 && color.G_norm < 0.14){
            card_memory[card_count] = 2; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,1); //Red card - Turn Right
         
        }
    
        if (color.B_norm < 0.25 && color.G_norm > 0.40) { 
            card_memory[card_count] = 1; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,2);   //Green card - Turn Left
        
        }
    
        if (color.R_norm < 0.38 && color.B_norm > 0.32 && color.G_norm > 0.34){
            card_memory[card_count] = 3; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,3);    //Blue card - Reverse 180
        
        }
    
        if (color.R_norm > 0.52 && color.G_norm > 0.32){
            card_memory[card_count] = 9; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,4);    //Yellow Card - Reverse 1 Square, Right turn
            
        }
    
        if (color.R_norm > 0.50 && color.B_norm > 0.24 && color.G_norm < 0.33){
            card_memory[card_count] = 10; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,5);    //Pink card
            
        }  
    
        if (color.R_norm > 0.60 && color.B_norm < 0.22 && color.G_norm > 0.23){
            card_memory[card_count] = 7; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,6);    //Orange
            
        }
    
        if (color.R_norm < 0.40 && color.B_norm > 0.30 && color.G_norm > 0.4){
            card_memory[card_count] = 6; // append the opposite movement number to the memory array for return home function
            card_count ++; //increment card count so the next position in the array can be accessed next time a card is flagged
            movement(mL,mR,7);    //Light Blue
            
        }
    
        if (color.R_norm < 0.5 && color.C > 16000  ){ //White card - Return home
           
                
            card_memory[card_count] = 3; //append 3 to array so the first move performed when return_home() is called is a 180 turn
            card_count ++; 
            
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
            Sleep();   
        
        }
        timer_reset(); // reset the timer so it represents only the time taken for the car to move forward to next card
    }
    
//    else if() { // wall (LOST)
//                    
//            card_memory[card_count] = 3; //append 3 to array so the first move performed when return_home() is called is a 180 turn
//            card_count ++; 
//            
//            LATGbits.LATG1 = 0; //red LED OFF
//            LATAbits.LATA4 = 0; //green LED OFF
//            LATFbits.LATF7 = 0; //blue LED OFF (Allows us to see if white card has been triggered)
//                
//            space(mL,mR); //Create space between wall and buggy
//            __delay_ms(500);
//            stop(mL,mR);
//            __delay_ms(500);
//            
//            return_home(mL,mR); // iterate through arrays to bring buggy back to start
//            
//            stop(mL,mR); 
//            __delay_ms(500);
//            
//            movement_return(mL,mR,3); //Lastly turn 180 so buggy is in exact start position
//            Sleep();
    
//            }           
        

    else {forward(mL,mR);} //If clear channel is below 2500, car will continue to move forward
}
    
/*****************************************
* Function used to bring buggy back to start position
* Iterates through timer array and card array from back to front 
*****************************************/
void return_home(DC_motor *mL, DC_motor *mR){
    while(timer_index > 0 && card_count > 0 ) { //Keep going till everything in array has been performed
        
        movement_return(mL,mR,card_memory[card_count-1]); //start with performing the opposite movement for the card that was read 
        card_count--; //reduce card count so previous position in array is accessed next time loop is performed
        __delay_ms(250);
        reverse(mL,mR); // reverse into the wall to align buggy perpendicular to wall
        __delay_ms(500);
        stop(mL,mR); //stabilise
        __delay_ms(500);
        forward(mL,mR);
        delay_ms_func(timer_memory[timer_index-1]); //move forward for the time dictated in the timer array
        timer_index--; //reduce timer index so previous position in array is accessed next time loop is performed
        stop(mL,mR);
        __delay_ms(200);
         reverse(mL,mR); // reverse just in case buggy is up against a wall and needs to make space
        __delay_ms(100);
        stop(mL,mR);        
        __delay_ms(250);

    }
}

/*****************************************
* Function used to create a custom delay for the timer
* Our timer overflows every 131 ms therfore this function takes a timer overflow input and creates the desired delay time
*****************************************/
void delay_ms_func(unsigned int time) {
    unsigned int i;
    for (i=0; i < time; i++) {__delay_ms(131);} //131 ms for each timer overflow
}

