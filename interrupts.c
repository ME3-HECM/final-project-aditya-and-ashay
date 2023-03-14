#include <xc.h>
#include "interrupts.h"
#include "serial.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void interrupts_init(void)
{

	
//    TRISBbits.TRISB1 = 1;   // Set TRIS value for B0
//    ANSELBbits.ANSELB1 = 0; // Turn off analogue input B0
//    interrupts_clear();     // Clear interrupts on the colour click module
//    
//    PIE0bits.INT1IE = 1;    // Enable external interrupt source 
//    IPR0bits.INT1IP = 1;    // Set clear channel interrupt to high priority 
    
    PIE0bits.TMR0IE = 1; //Enable Timer interrupt source
    PIR0bits.TMR0IF = 0; //Clear interrupt flag
    IPR0bits.TMR0IP = 0;  // Set timer interrupt to low priority
    
//    INTCONbits.INT1EDG = 0; // Set interrupt on falling edge
    INTCONbits.IPEN = 1;    // Enable priority levels on interrupts
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts last, once all other interrupt configuration is done (when this is off, all interrupts are deactivated)
}


//void colorclick_interrupts_init(void) {
//    color_writetoaddr(0x00,0b10011);                          // write to enable register and turn on interrupts                         
//    color_writetoaddr(0x04, (color_lowerbound & 0xFF)); // low bits for lower colour threshold
//    color_writetoaddr(0x05, (color_lowerbound >> 8));     // high bits for lower colour threshold
//    color_writetoaddr(0x06, (color_upperbound & 0xFF)); // set low bits for upper threshold
//    color_writetoaddr(0x07, (color_upperbound >> 8));     // set high bits for upper threshold
//    color_writetoaddr(0x0C, 0b0001); //Persistence register set to 1
//}

//void interrupts_clear(void){
//    I2C_2_Master_Start();            // Start condition
//    I2C_2_Master_Write(0x52 | 0x00); // 7 bit device address + Write mode
//    I2C_2_Master_Write(0b11100110);  // Command + Register address  
//    I2C_2_Master_Stop();             // Send stop bit
//}

//void __interrupt(high_priority) HighISR() {
//   if  (PIR0bits.INT0IF) {
//       interrupts_clear();
//       PIR0bits.INT0IF = 0; //Clear the interrupt flag
//   }      
//}

void __interrupt(low_priority) LowISR() 
{
    if (PIR0bits.TMR0IF) {
        timer_val += 1;
        PIR0bits.TMR0IF = 0;
    }
  
}





