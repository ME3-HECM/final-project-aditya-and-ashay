
#include <xc.h>
#include "lights_buttons.h"

void ports_init(void){
        
    TRISGbits.TRISG1 = 0; // Set TRIS value for red LED (output)
    TRISAbits.TRISA4 = 0; // Set TRIS value for green LED (output)
    TRISFbits.TRISF7 = 0; // Set TRIS value for blue LED (output)
    
    LATGbits.LATG1 = 1; //red LED on
    LATAbits.LATA4 = 1; //green LED on
    LATFbits.LATF7 = 1; //blue LED on
    
    TRISFbits.TRISF2=1; //set TRIS value for pin (input)  
    ANSELFbits.ANSELF2=0; //turn off analogue input on pin
    
    TRISFbits.TRISF3= 1;
    ANSELFbits.ANSELF3 = 0;
    
    TRISDbits.TRISD7 = 0;
    LEDD7 = 0;
    
    TRISHbits.TRISH3 = 0;
    LEDH3 = 0;
}

void buggyLEDs_init(void) {
    HEADLAMPS = 0;    // Set initial LAT value for H.LAMPS
    MAINLIGHT = 0;     // Set initial LAT value for M.BEAM
    BRAKELIGHT = 0;        // Set initial LAT value for BRAKE
    LEFTINDICATOR = 0;     // Set initial LAT value for TURN-L
    RIGHTINDICATOR = 0;    // Set initial LAT value for TURN-R
    
    TRISHbits.TRISH1 = 0; // Set TRIS value for H.LAMPS
    TRISDbits.TRISD3 = 0; // Set TRIS value for M.BEAM
    TRISDbits.TRISD4 = 0; // Set TRIS value for BRAKE
    TRISFbits.TRISF0 = 0; // Set TRIS value for TURN-L
    TRISHbits.TRISH0 = 0; // Set TRIS value for TURN-R
}
