/*  
 * Code for controlling robot movement  
 * Arduino: mega
*/  
#include "Logic.h"

/*
 * Pins list
 * 
 *  2, 3  - voice recognition
 *  4..8  - ultrasonic pins
 *  9..12 - movement relays
 * 13..15 - ultrasonic pins
 */

#define FIRSTCONTROLLPIN 9  
#define SECONDCONTROLLPIN 10  
#define THIRDCONTROLLPIN 11 
#define FOURTHCONTROLLPIN 12

#define ULTRASONIC_ZERO_TRIG 4  
#define ULTRASONIC_ZERO_ECHO 5  
#define ULTRASONIC_FIRST_TRIG 6 
#define ULTRASONIC_FIRST_ECHO 7 
#define ULTRASONIC_SECOND_TRIG 8  
#define ULTRASONIC_SECOND_ECHO 13  
#define ULTRASONIC_THIRD_TRIG 14
#define ULTRASONIC_THIRD_ECHO 15

// Main core  

Logic *core;

void setup()  
{ 
  short unsigned int ultrasonic[19]; 
  for (short unsigned int i = 0; i < 20; i++) ultrasonic[i] = 0; 
  ultrasonic[0] = ULTRASONIC_ZERO_TRIG; 
  ultrasonic[1] = ULTRASONIC_ZERO_ECHO; 
  ultrasonic[2] = ULTRASONIC_FIRST_TRIG;  
  ultrasonic[3] = ULTRASONIC_FIRST_ECHO;  
  ultrasonic[4] = ULTRASONIC_SECOND_TRIG; 
  ultrasonic[5] = ULTRASONIC_SECOND_ECHO;
  ultrasonic[6] = ULTRASONIC_THIRD_TRIG;
  ultrasonic[7] = ULTRASONIC_THIRD_ECHO; 

  core = new Logic( 
    FIRSTCONTROLLPIN,   
    SECONDCONTROLLPIN,  
    THIRDCONTROLLPIN,  
    FOURTHCONTROLLPIN,  
    ultrasonic  
  );    

  core->init();
} 

void loop()   
{
  core->readEnternControlls();
}
