#include <xc.h>
#include "color.h"
#include "i2c.h"
#include "dc_motor.h"

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
    // For line below Red = 0x16, Blue = 0x1A (0x01 according to Amanda), Green = 0x18, Clear = 0x14
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

void READcolor(void) {
    
    colors.R = color_read_Red();
    colors.B = color_read_Blue();
    colors.G = color_read_Green();
    colors.C = color_read_Clear();
    
    colors.R_norm = colors.R / colors.C;
    colors.B_norm = colors.B / colors.C;
    colors.G_norm = colors.G / colors.C;
}

void buggy_color_response(DC_motor *mL, DC_motor *mR) {
    READcolor();
    if (colors.C > 2500) {
        
        if (colors.R_norm > 0.77 && colors.B_norm < 0.175 && colors.G_norm < 0.135){
        instructions(mL, mR, 1);} //RED COLOUR CARD
        
        if ( colors.B_norm < 0.25 && colors.G_norm > 0.4) {
           //GReen card
        }
        if (colors.R_norm < 0.38 && colors.B_norm > 0.32 && colors.G_norm > 0.34){
            //Blue card
        }
        
            
    } 
}
