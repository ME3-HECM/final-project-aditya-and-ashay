
#include <xc.h>
#include "lights_buttons.h"

/************************************************************
* Function used to initialise all the pins and buttons needed
************************************************************/
void ports_init(void){
        
    TRISGbits.TRISG1 = 0; // Set TRIS value for red LED (output)
    TRISAbits.TRISA4 = 0; // Set TRIS value for green LED (output)
    TRISFbits.TRISF7 = 0; // Set TRIS value for blue LED (output)
            
    LATGbits.LATG1 = 1; //red LED on (colour click)
    LATAbits.LATA4 = 1; //green LED on (colour click)
    LATFbits.LATF7 = 1; //blue LED on (colour click)
    
    TRISFbits.TRISF2=1;   
    ANSELFbits.ANSELF2=0; //Button RF2
    
    TRISFbits.TRISF3= 1;
    ANSELFbits.ANSELF3 = 0; //Button RF3
    
    TRISDbits.TRISD7 = 0;
    LEDD7 = 0;
    
    TRISHbits.TRISH3 = 0;
    LEDH3 = 0;
}

/************************************************************
* Function used to initialise all the pins for the lights on the buggy
************************************************************/
void buggyLEDs_init(void) {
    HEADLAMPS = 0;   
    MAINLIGHT = 0;     
    BRAKELIGHT = 0;        
    LEFTINDICATOR = 0;     
    RIGHTINDICATOR = 0;    
    
    TRISHbits.TRISH1 = 0; 
    TRISDbits.TRISD3 = 0; 
    TRISDbits.TRISD4 = 0; 
    TRISFbits.TRISF0 = 0; 
    TRISHbits.TRISH0 = 0; 
}
