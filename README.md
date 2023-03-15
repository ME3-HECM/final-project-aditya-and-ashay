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

- Link for the easy maze:
- Link for the hard maze:
- Link for dead end reaction:

## Hardware setup

## Calibrations

One of the most important elements of this project was calibrating the buggy to dynamically varying surfaces and ambient light. This was necessary in order to perform the exact turns and read the colours perfectly. To assist in improving the buggy's performance, the rubber on the tyres were removed as it allowed more consistent turns when compared with the rubber on. There was also a special black sleeve which was 3-D printed to allow better colour reading. The calibration on test day was a 2 step process which went as follows:

### 1) Colour calibration

Colour calibration was performed once before the testing day and one during the test day to confirm if any changes were necessary in the code. To calibrate the colour, the Red, Green, Blue, and Clear values are read from the array of sensors built into the buggy's chip. The sensor contains a grid of 4x4 photodiodes, 4 are sensitive to red light, 4 green light, 4 blue light and 4 "clear" light (that is, a range of wavelengths, see datasheet for exact spectral response). When light falls on the photodiode the photons are absorbed and current is generated. This signal is then integrated over time using integrators and sampled by 16 bit on board ADCs. Communication with the device is achieved using an I2C interface. This enables configuration of the device to customise sampling of the data (i.e. integration time, gain, etc.) and to read the 16 bit digital values for each of the RGBC channels. The relative magnitude of these values gives you information about the colour of light that is falling on the sensor. 

We decided to have no gap between the buggy's sensors and the colour card so that the readings can be precise. We then measured over a 100 Red, Green, Blue and Clear values produced in multiple locations and time (day, night, evening, on the 7th floor, at home etc.) and stored these values in a text file with the help of the REALTERM software. 

We then imported these values in an excel sheet and for each colour we found normalised Red, Green and Blue values. This was done by dividing each individual colour by the Clear value and gave consistent results for multiple operating conditions. This process was repeated during test day with fewer data to confirm if the values fell into the right categories.

![Excel individual colour values](https://user-images.githubusercontent.com/89412018/225194822-102c9d81-736a-4792-b63d-f6904ccc3c89.JPG)


### 2) Turning calibration

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

- Link for turning calibration: 

## Code structure


### I2C

The I2C interface widely used in industry for communication between microcontrollers and peripheral integrated circuits (other chips) over short distances. I2C is serial communication bus that enables communication between many devices over a simple 2 wire interface. One wire is the data line (SDA) and is used for both transmission and receiving. The second wire (SCL) is used for a clock signal to ensure all devices are synchronous. To ensure communication of data occurs without problem a message protocol must be followed and understood by all devices on the bus. Devices are termed master and slave devices, with master devices initiation communication to a slave device via unique address for that device. The general sequence of communication between a master/slave over the I2C interface is a follows:

1. Send a Start bit
1. Send the slave address, usually 7 bits
1. Send a Read (1) or Write (0) bit to define what type of transaction it is
1. Wait for an Acknowledge bit
1. Send a data or command byte (8 bits)
1. Wait for Acknowledge bit
1. Send the Stop bit

This is shown pictorial in the figure below:

![I2C](gifs/i2c.png)

Although it is possible to program an entirely software based I2C interface, luckily for us our PIC chip has a module dedicated to generating and receiving I2C signals: the Master Synchronous Serial Port Module, or MSSP (see chapter 28 of the PIC datasheet). This module provides methods for sending start/stop/acknowledge bits and allows us to focus on sending/receiving data.

The included i2c.c/h files contain functions to help you get started with I2C communication. The first function below sets up the MSSP module as an I2C master device and configures the necessary pins.

	void I2C_2_Master_Init(void)
	{
		//i2c config  
		SSP2CON1bits.SSPM= 0b1000;    // i2c master mode
		SSP2CON1bits.SSPEN = 1;       //enable i2c
		SSP2ADD = (_XTAL_FREQ/(4*_I2C_CLOCK))-1; //Baud rate divider bits (in master mode)
  
		//pin configuration for i2c  
		TRISDbits.TRISD5 = 1;                   //Disable output driver
		TRISDbits.TRISD6 = 1;                   //Disable output driver
		ANSELDbits.ANSELD5=0;					// disable analogue on pins
		ANSELDbits.ANSELD6=0;					// disable analogue on pins
		SSP2DATPPS=0x1D;      //pin RD5
		SSP2CLKPPS=0x1E;      //pin RD6
		RD5PPS=0x1C;      // data output
		RD6PPS=0x1B;      //clock output
	}
	
Bits with the SSP2CON2 register are set to send the individual start/stop/acknowledge bits used in the protocol. These must only be set when the bus is idle (nothing being sent/received). The I2C_2_Master_Start(), I2C_2_Master_Stop() and I2C_2_Master_RepStart() functions allow you add the necessary bits as defined in the protocol above. Data is sent on the bus using the SSP2BUF register:

	void I2C_2_Master_Write(unsigned char data_byte)
	{
		I2C_2_Master_Idle();
		SSP2BUF = data_byte;         //Write data to SSPBUF
	}

Data is also read using the same SSP2BUF registers. However, to receive data we first need to switch the module into receiver mode. We also need to start the acknowledge sequence to let the slave device know what we have received the data OK. The following function achieves this:

	unsigned char I2C_2_Master_Read(unsigned char ack)
	{
		unsigned char tmp;
		I2C_2_Master_Idle();
		SSP2CON2bits.RCEN = 1;        // put the module into receive mode
		I2C_2_Master_Idle();
		tmp = SSP2BUF;                //Read data from SS2PBUF
		I2C_2_Master_Idle();
		SSP2CON2bits.ACKDT = !ack;     // 0 turns on acknowledge data bit
		SSP2CON2bits.ACKEN = 1;        //start acknowledge sequence
		return tmp;
	}

The functions described so form the basics required for I2C communication with the PIC. To communicate with the TCS3471 onboard the Colour click we first need to know its address. This is listed in the data sheet as 0x29. To send our first byte over the I2C we need to send this address combined with the transaction type (read or write) as defined in the protocol diagram above. This lets the TCS3471 know the message is intended for it and not some other device on the interface. Next we send a byte which is a combination of command type and the register address in the TCS3471 that we want to write to. Finally we the value that we want to write to that register. This sequence is shown in the function below:

	void color_writetoaddr(char address, char value){
		I2C_2_Master_Start();         		//Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write (0) mode (note 0x52=0x29<<1)
		I2C_2_Master_Write(0x80 | address);    //command + register address
		I2C_2_Master_Write(value);    			//value to store in the register
		I2C_2_Master_Stop();          //Stop condition
	}

We then call the function to, for example, turn the device on:

	color_writetoaddr(0x00, 0x01); // write 1 to the PON bit in the device enable register
	
There are additional commands that must be set to initialise the device and many registers that be configured to obtain optimal performance for the sensor in your conditions. It is up to you to carefully read the TCS3471 datasheet and experiment with this.

To read values from the TCS3471 you need to a similar sequence to above but you first need to tell the device which register you want to deal with, before telling the device you want read from it. The example below uses the combined read format to read multiple bytes in sequence. The auto-increment is set so that instead of reading the same register over and over again, it automatically advances to the next one. The example starts at the Red channel low byte address and then automatically advances and reads the associated high byte.

	unsigned int color_read_Red(void)
	{
		unsigned int tmp;
		I2C_2_Master_Start();         //Start condition
		I2C_2_Master_Write(0x52 | 0x00);     //7 bit address + Write mode
		I2C_2_Master_Write(0xA0 | 0x16);    //command (auto-increment protocol transaction) + start at RED low register
		I2C_2_Master_RepStart();
		I2C_2_Master_Write(0x52 | 0x01);     //7 bit address + Read (1) mode
		tmp=I2C_2_Master_Read(1);			// read the Red LSB
		tmp=tmp | (I2C_2_Master_Read(0)<<8); //read the Red MSB (don't acknowledge as this is the last read)
		I2C_2_Master_Stop();          //Stop condition
		return tmp;
	}

Instead of separate functions for each channel you may want to create a structure to store all the values together, and pass a pointer to the function so that all values in the structure can be updated in one operation. An example structure might look like this:

	//definition of RGB structure
	struct RGB_val { 
		unsigned int R;
		unsigned int G;
		unsigned int B;
	};

This concludes the basics of I2C and communication with the colour sensor. Best of luck! 
