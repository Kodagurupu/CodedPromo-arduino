/*  
 * Code for controlling robot movement  
*/  
#include "Logic.h"
#include "tests/voiceTest.h"

#define FIRSTCONTROLLPIN 9  
#define SECONDCONTROLLPIN 10  
#define BLUETOOTHPIN_ZERO 0 
#define BLUETOOTHPIN_ONE 1  

#define ULTRASONIC_ZERO_TRIG 3  
#define ULTRASONIC_ZERO_ECHO 4  
//#define ULTRASONIC_FIRST_TRIG 5 
//#define ULTRASONIC_FIRST_ECHO 6 
//#define ULTRASONIC_SECOND_TRIG 7  
//#define ULTRASONIC_SECOND_ECHO 8  

// Main core  

Logic *core;  
VoiceTest *voice;

void setup()  
{ 
  pinMode(8, OUTPUT); 

  int ultrasonic[19]; 
  for (int i = 0; i < 20; i++) ultrasonic[i] = 0; 
  ultrasonic[0] = ULTRASONIC_ZERO_TRIG; 
  ultrasonic[1] = ULTRASONIC_ZERO_ECHO; 
  //ultrasonic[2] = ULTRASONIC_FIRST_TRIG;  
  //ultrasonic[3] = ULTRASONIC_FIRST_ECHO;  
  //ultrasonic[4] = ULTRASONIC_SECOND_TRIG; 
  //ultrasonic[5] = ULTRASONIC_SECOND_ECHO; 

  core = new Logic( 
    FIRSTCONTROLLPIN,   
    SECONDCONTROLLPIN,  
    BLUETOOTHPIN_ZERO,  
    BLUETOOTHPIN_ONE,  
    ultrasonic  
  );    

  core->init(); 
  voice = new VoiceTest();
  //core->check();  

  pinMode(8, OUTPUT); 
} 

void loop()   
{ 
  voice->start();
}
