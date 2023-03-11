#include <xc.h>
#include "interrupts.h"
#include "color.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void interrupts_init(void)
{
    TRISBbits.TRISB1 = 1;   // Set B0 to input
    ANSELBbits.ANSELB1 = 0; // Turn off analogue input for B0
    
    PIE0bits.INT1IE = 1;    // Enable external interrupt source
    
    IPR0bits.INT1IP = 1;    // Set interrupt to high priority 
    
    INTCONbits.INT1EDG = 0; // interrupt on falling edge
    INTCONbits.IPEN = 1;    // Enable priority levels 
    
    INTCONbits.PEIE=1; 	//enable peripheral interrupts
    INTCONbits.GIE=1; 	//enable interrupts globally 
	
    colorclick_interrupts_init();
}

void colorclick_interrupts_init(void) {
    color_writetoaddr(0x00,0x13);                          // write to enable register and turn on interrupts                         
    color_writetoaddr(0x04, (color_lowerbound & 0x00FF)); // low bits for lower colour threshold
    color_writetoaddr(0x05, (color_lowerbound >> 8));     // high bits for lower colour threshold
    color_writetoaddr(0x06, (color_upperbound & 0x00FF)); // set low bits for upper threshold
    color_writetoaddr(0x07, (color_upperbound >> 8));     // set high bits for upper threshold
    color_writetoaddr(0x0C, 0b0011); //Persistence register set to 3
}

void __interrupt(high_priority) HighISR() {
   if  (PIR0bits.INT0IF) {
       read_color_flag = 1;
       PIR0bits.INT0IF = 0; //Clear the interrupt flag
   }      
}
