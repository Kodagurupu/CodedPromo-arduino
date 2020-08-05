/*
 * Code for controlling robot movement 
*/
#include "Logic.h"

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

#define SERVOPIN0 5
#define SERVOPIN1 6

// Main core

Logic *core;

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
    SERVOPIN0,
    SERVOPIN1,
    ultrasonic
  );  
  
  core->init();
  core->check();

  pinMode(8, OUTPUT);
}

void loop() 
{
  Serial.println("Blyat");
  if (Serial.available() > 0) {
    int charset = Serial.parseInt();
    Serial.println(charset);
    if (charset == 1) {
      Serial.println("YAAAAAAAAA");
      digitalWrite(8, HIGH);
      delay(2000);
    } else {
      digitalWrite(8, LOW);
    }
  }
  delay(1000);
}
