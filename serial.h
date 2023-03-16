#ifndef _SERIAL_H
#define _SERIAL_H

#include <xc.h>

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

#define RX_BUF_SIZE 20
#define TX_BUF_SIZE 60


//variables for a software RX/TX buffer
//volatile char EUSART4RXbuf[RX_BUF_SIZE];
//volatile char RxBufWriteCnt=0;
//volatile char RxBufReadCnt=0;
//
//volatile char EUSART4TXbuf[TX_BUF_SIZE];
//volatile char TxBufWriteCnt=0;
//volatile char TxBufReadCnt=0;


//basic EUSART funcitons

/************************************
 * Function to intiliase USART for data transmission
************************************/
void initUSART4(void);

/************************************
 * Function to wait for a byte to arrive on serial port and read it once it does 
************************************/
char getCharSerial4(void);

/************************************
 * Function to check the TX reg is free and send a byte
************************************/
void sendCharSerial4(char charToSend);

/************************************
 * Function to send a string over the serial interface
************************************/
void sendStringSerial4(char *string);



#endif
