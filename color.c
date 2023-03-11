#include <xc.h>             // Include processor file
#include <stdio.h>          // Include standard input output library to use serial communication for testing purposes
#include "color.h"
#include "i2c.h"
#include "dc_motor.h"
#include "interrupts.h"

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

void color_writetoaddr(char address, char value){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0x80 | address);    //command + register address
    I2C_2_Master_Write(value);    
    I2C_2_Master_Stop();          //Stop condition
}

unsigned int color_read_Red(void)
{
	unsigned int tmp;
	I2C_2_Master_Start();         //Start condition
	I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
    // For line below Red = 0x16, Blue = 0x1A, Green = 0x18, Clear = 0x14
	I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
	I2C_2_Master_RepStart();			// start a repeated transmission
	I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
	tmp=I2C_2_Master_Read(1);			//read the Red LSB
	tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
	I2C_2_Master_Stop();          //Stop condition
	return tmp;
}

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

void READcolor(colors *c) {
    
    color.R = color_read_Red();
    color.B = color_read_Blue();
    color.G = color_read_Green();
    color.C = color_read_Clear();
    
//    color.R_norm = (float)color.R / (float)color.C;
//    color.B_norm = (float)color.B / (float)color.C;
//    color.G_norm = (float)color.G / (float)color.C;
}

void buggy_color_response(DC_motor *mL, DC_motor *mR, colors *c) {
    READcolor(&color);
    colourcards_normaliseRGBC(&color);
    if (color.C > 2500){stop(mL,mR);__delay_ms(500);READcolor(&color);colourcards_normaliseRGBC(&color);__delay_ms(500);
    if (color.R_norm > 0.77 && color.B_norm < 0.18 && color.G_norm < 0.14){
    instructions(mL,mR,1); //RED COLOUR CARD
    }
    if (color.B_norm < 0.25 && color.G_norm > 0.40) {
    instructions(mL,mR,2);   //Green card
    }
    if (color.R_norm < 0.38 && color.B_norm > 0.32 && color.G_norm > 0.34){
    instructions(mL,mR,3);    //Blue card
    }
    if (color.R_norm > 0.52 && color.G_norm > 0.32){
    instructions(mL,mR,4);    //Yellow Card
    }
    if (color.R_norm > 0.50 && color.B_norm > 0.24 && color.G_norm < 0.33){
    instructions(mL,mR,5);    //Pink card
    }  
    if (color.R_norm > 0.60 && color.B_norm < 0.22 && color.G_norm > 0.23){
    instructions(mL,mR,6);    //Orange
    }
    if (color.R_norm < 0.40 && color.B_norm > 0.30 && color.G_norm > 0.4){
    instructions(mL,mR,7);    //Light Blue
    }
    if (color.R_norm < 0.5 && color.C > 16000){
    //White
    }
    }
    else {forward(mL,mR);}
    read_color_flag = 0;
} 


void colourcards_normaliseRGBC(colors *c)
{
    unsigned int R = color.R;
    unsigned int G = color.G;
    unsigned int B = color.B;
    unsigned int C = color.C;
    
    color.R_norm = (float)R/(float)C;
    color.G_norm = (float)G/(float)C;
    color.B_norm = (float)B/(float)C;
}