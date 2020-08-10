/*
 * Code for controlling robot movement 
*/
#include "Logic.h"

#define ULTRASONIC_ZERO_TRIG 3
#define ULTRASONIC_ZERO_ECHO 4
/*
/#define ULTRASONIC_FIRST_TRIG 5
/#define ULTRASONIC_FIRST_ECHO 6
/#define ULTRASONIC_SECOND_TRIG 7
/#define ULTRASONIC_SECOND_ECHO 8
*/
#define FIRSTCONTROLLPIN 9
#define SECONDCONTROLLPIN 10
#define THIRDCONTROLLPIN 11
#define FOURTHCONTROLLPIN 12

// Init main core
Logic *core;

void setup() 
{  
  int ultrasonic[19];
  for (int i = 0; i < 20; i++) ultrasonic[i] = 0;
  ultrasonic[0] = ULTRASONIC_ZERO_TRIG;
  ultrasonic[1] = ULTRASONIC_ZERO_ECHO;
  /* Declare ultrasonic array
   * ultrasonic[2] = ULTRASONIC_FIRST_TRIG;
   * ultrasonic[3] = ULTRASONIC_FIRST_ECHO;
   * ultrasonic[4] = ULTRASONIC_SECOND_TRIG;
   * ultrasonic[5] = ULTRASONIC_SECOND_ECHO;  
  */

  /* Declare core
   *  FIRSTCONTROLLPIN  rele num 1
   *  SECONDCONTROLLPIN rele num 3
   *  THIRDCONTROLLPIN  rele num 3
   *  FOURTHCONTROLLPIN rele num 4
   *  ultrasonic        array of ultrasonic pins
   */
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
  core->ReadSerial();
  delay(500);
}
