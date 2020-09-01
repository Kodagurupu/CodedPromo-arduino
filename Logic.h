#ifndef LOGIC_H
#define LOGIC_H

#include "Moving.h"
#include "Voice.h"
#include "Ultrasonic.h"

#define DEBUGMODE true

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
  void sendCommand(short unsigned int command);
  void readEnternControlls();
  void stopMoving();
  void turnLeft();
  void turnRight();
  void goForward();
  void goBackward();

private:
  Voice voice;
  Moving moving;
  Ultrasonic ultrasonic;
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

  voice.init();
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

void Logic::readEnternControlls()
{
  // Read com port from laptop
  if ( Serial.available() > 0 )
  {
    int command = Serial.parseInt();
    sendCommand(command);
  }
  // Read voice recognation module
  short unsigned int command = voice.loopFunc();
  sendCommand(command - 1);
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
