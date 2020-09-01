#ifndef LOGIC_H
#define LOGIC_H

#include "Moving.h"
#include "voice.h"

#define DEBUGMODE true
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

class Logic 
{  
public:
  Logic(
    short unsigned int pin1, 
    short unsigned int pin2,
    short unsigned int pin3,
    short unsigned int pin4,
    short unsigned int ultrasonic[19]
  );
  void init();
  void ultraSonicBlink(short unsigned int i);
  void sendCommand(short unsigned int command);
  void readSerial();
  void stopMoving();
  void turnLeft();
  void turnRight();
  void goForward();
  void goBackward();
  bool globalRules();

private:
  Voice voice;
  Moving moving;
  short unsigned int ultraSonicPins[19];
};

Logic::Logic (
  short unsigned int pin1, 
  short unsigned int pin2,
  short unsigned int pin3,
  short unsigned int pin4,
  short unsigned int ultrasonic[19]
) 
  : moving(
      pin1,
      pin2,
      pin3,
      pin4
    )
{
  // init ultrasonic pins
  for (short unsigned int i = 0; i < 20; i++) 
  {
    if (ultrasonic[i] != 0) 
      ultraSonicPins[i] = ultrasonic[i];
    else ultraSonicPins[i] = 0;      
  } 
}

void Logic::ultraSonicBlink(short unsigned int i)
{
  digitalWrite(ultraSonicPins[i], LOW);
  delayMicroseconds(ULTRASONICDELAY_LOW);
  digitalWrite(ultraSonicPins[i], HIGH);
  delayMicroseconds(ULTRASONICDELAY_HIGH);
  digitalWrite(ultraSonicPins[i], LOW);
}

bool Logic::globalRules()
{
  // skip this to future realize
  
  return true;
  for (short unsigned int i = 0; i < 19; i++) 
  {
    if (ultraSonicPins[i] != 0 && i % 2 == 0) 
    {
      long duration0, duration1, distance;
      ultraSonicBlink(i);
      duration0 = pulseIn(ultraSonicPins[i+1], HIGH);
      ultraSonicBlink(i);
      duration1 = pulseIn(ultraSonicPins[i+1], HIGH);
      distance = ((duration0 + duration1) / 2) * 0.034/2;
      if (distance < 10) return false;
    }
  }
  return true;
}

void Logic::init()
{
  //Init Seral
  
  Serial.begin(9600);
  Serial.println("[CORE] Initializing");  
   
  for (short unsigned int i = 0; i < 20; i++) 
  {
    if (ultraSonicPins[i] != 0) 
    {
      if ( i % 2 == 0 ) pinMode(ultraSonicPins[i], OUTPUT);
      else pinMode(ultraSonicPins[i], INPUT);
    }      
  }
  delay(500);
}

void Logic::sendCommand(short unsigned int command)
{
  if ( command == -1 ) stopMoving();
  if ( command == 0 ) goForward();
  if ( command == 1 ) goBackward();
  if ( command == 2 ) turnLeft();
  if ( command == 3 ) turnRight();
}

void Logic::readSerial()
{
  if ( Serial.available() > 0 )
  {
    int command = Serial.parseInt();
    sendCommand(command);
  }
}

void Logic::stopMoving()
{
  if (DEBUGMODE) Serial.println("stoping");
  moving.restore();
}

void Logic::turnLeft()
{
  if (DEBUGMODE) Serial.println("Turning left");
  moving.turnLeft();  
}

void Logic::turnRight()
{
  if (DEBUGMODE) Serial.println("Turning right");
  moving.turnRight();
}

void Logic::goForward()
{
  if (DEBUGMODE) Serial.println("Moving forward");
  moving.goFoward();
}

void Logic::goBackward()
{
  if (DEBUGMODE) Serial.println("Moving backward");
  moving.goBackward();
}

#endif
