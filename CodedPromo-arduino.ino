/*  
 * Code for controlling robot movement  
 * Arduino: mega
*/  
#include "Logic.h"
#include "Ultrasonic.h"

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
#define ULTRASONIC_THIRD_TRIG 22
#define ULTRASONIC_THIRD_ECHO 23

// Main core  

Logic *core;

void setup()  
{ 
  Module modules[4];
  struct Module firstFrontModule = 
  { 
    .type = FRONT, 
    .trigPin = ULTRASONIC_ZERO_TRIG, 
    .echoPin = ULTRASONIC_ZERO_ECHO 
  };
  struct Module secondFrontModule = 
  { 
    .type = FRONT, 
    .trigPin = ULTRASONIC_FIRST_TRIG, 
    .echoPin = ULTRASONIC_FIRST_ECHO 
  };
  struct Module thirdFrontModule = 
  { 
    .type = FRONT, 
    .trigPin = ULTRASONIC_SECOND_TRIG, 
    .echoPin = ULTRASONIC_SECOND_ECHO 
  };

  struct Module firstBackModule = 
  { 
    .type = BACK, 
    .trigPin = ULTRASONIC_THIRD_TRIG, 
    .echoPin = ULTRASONIC_THIRD_ECHO 
  };
 
  modules[0] = firstFrontModule;
  modules[1] = secondFrontModule;
  modules[2] = thirdFrontModule;
  modules[3] = firstBackModule;
  
  core = new Logic( 
    FIRSTCONTROLLPIN,   
    SECONDCONTROLLPIN,  
    THIRDCONTROLLPIN,  
    FOURTHCONTROLLPIN,  
    modules  
  );    

  core->init();
  core->toggleMode(Walking);
} 

void loop()   
{
 //core->stopMoving();
  core->loopFunc();
}
